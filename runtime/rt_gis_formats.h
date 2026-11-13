/*
 * rt_gis_formats.h — Native Binary Shapefile (.shp) and GeoTIFF (.tif) Parsers
 * Part of the Nyx Geospatial Engine (Zero External Dependencies)
 */

#ifndef RT_GIS_FORMATS_H
#define RT_GIS_FORMATS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ── 1. Byte-Order Swapping Helpers (Big-Endian <-> Little-Endian) ───────── */
static inline uint32_t nyx_swap32(uint32_t v) {
    return ((v & 0xFF000000) >> 24) |
           ((v & 0x00FF0000) >> 8)  |
           ((v & 0x0000FF00) << 8)  |
           ((v & 0x000000FF) << 24);
}

static inline uint16_t nyx_swap16(uint16_t v) {
    return (uint16_t)(((v & 0xFF00) >> 8) | ((v & 0x00FF) << 8));
}

static inline double nyx_swap64_double(double v) {
    union { double d; uint64_t u; } src, dst;
    src.d = v;
    dst.u = ((src.u & 0xFF00000000000000ULL) >> 56) |
            ((src.u & 0x00FF000000000000ULL) >> 40) |
            ((src.u & 0x0000FF0000000000ULL) >> 24) |
            ((src.u & 0x000000FF00000000ULL) >> 8)  |
            ((src.u & 0x00000000FF000000ULL) << 8)  |
            ((src.u & 0x0000000000FF0000ULL) << 24) |
            ((src.u & 0x000000000000FF00ULL) << 40) |
            ((src.u & 0x00000000000000FFULL) << 56);
    return dst.d;
}

/* ── 2. ESRI Shapefile (.shp) Binary Specification ────────────────────────── */
#define NYX_SHP_FILE_CODE       9994
#define NYX_SHP_VERSION         1000

#define NYX_SHAPE_NULL          0
#define NYX_SHAPE_POINT         1
#define NYX_SHAPE_POLYLINE      3
#define NYX_SHAPE_POLYGON       5
#define NYX_SHAPE_MULTIPOINT    8

#pragma pack(push, 1)
typedef struct {
    uint32_t file_code;       /* Big Endian: 9994 */
    uint32_t unused[5];       /* Big Endian: Unused */
    uint32_t file_length;     /* Big Endian: 16-bit words */
    uint32_t version;         /* Little Endian: 1000 */
    uint32_t shape_type;      /* Little Endian: 1=Point, 3=PolyLine, 5=Polygon */
    double   x_min;           /* Little Endian: Bounding Box */
    double   y_min;
    double   x_max;
    double   y_max;
    double   z_min;           /* Little Endian: 0.0 */
    double   z_max;
    double   m_min;           /* Little Endian: 0.0 */
    double   m_max;
} NyxShpHeader;

typedef struct {
    uint32_t record_number;   /* Big Endian (1-based) */
    uint32_t content_length;  /* Big Endian (in 16-bit words) */
} NyxShpRecordHeader;
#pragma pack(pop)

typedef struct {
    double x;
    double y;
} NyxPoint2D;

#define NYX_MAX_SHP_VERTICES 256

typedef struct {
    int32_t record_id;
    int32_t shape_type;
    double  x_min, y_min, x_max, y_max;
    int32_t num_parts;
    int32_t num_points;
    int32_t parts[8];
    NyxPoint2D points[NYX_MAX_SHP_VERTICES];
} NyxShpFeature;

#define NYX_MAX_SHP_FEATURES 64

typedef struct {
    NyxShpHeader header;
    size_t       feature_count;
    NyxShpFeature features[NYX_MAX_SHP_FEATURES];
} NyxShpDataset;

/* Write an ESRI Shapefile to disk */
static inline bool nyx_shp_write(const char* filepath, const NyxShpDataset* ds) {
    FILE* fp = fopen(filepath, "wb");
    if (!fp) return false;

    /* Calculate total file length in 16-bit words */
    uint32_t total_bytes = 100;
    for (size_t i = 0; i < ds->feature_count; i++) {
        const NyxShpFeature* f = &ds->features[i];
        uint32_t rec_content_bytes = 0;
        if (f->shape_type == NYX_SHAPE_POINT) {
            rec_content_bytes = 4 + 16;
        } else if (f->shape_type == NYX_SHAPE_POLYGON || f->shape_type == NYX_SHAPE_POLYLINE) {
            rec_content_bytes = 4 + 32 + 4 + 4 + (f->num_parts * 4) + (f->num_points * 16);
        }
        total_bytes += 8 + rec_content_bytes;
    }

    NyxShpHeader hdr;
    hdr.file_code = nyx_swap32(NYX_SHP_FILE_CODE);
    memset(hdr.unused, 0, sizeof(hdr.unused));
    hdr.file_length = nyx_swap32(total_bytes / 2);
    hdr.version = NYX_SHP_VERSION;
    hdr.shape_type = ds->header.shape_type;
    hdr.x_min = ds->header.x_min;
    hdr.y_min = ds->header.y_min;
    hdr.x_max = ds->header.x_max;
    hdr.y_max = ds->header.y_max;
    hdr.z_min = 0.0;
    hdr.z_max = 0.0;
    hdr.m_min = 0.0;
    hdr.m_max = 0.0;

    fwrite(&hdr, sizeof(NyxShpHeader), 1, fp);

    /* Write records */
    for (size_t i = 0; i < ds->feature_count; i++) {
        const NyxShpFeature* f = &ds->features[i];
        uint32_t rec_content_bytes = 0;
        if (f->shape_type == NYX_SHAPE_POINT) {
            rec_content_bytes = 4 + 16;
        } else {
            rec_content_bytes = 4 + 32 + 4 + 4 + (f->num_parts * 4) + (f->num_points * 16);
        }

        NyxShpRecordHeader rec_hdr;
        rec_hdr.record_number = nyx_swap32((uint32_t)(i + 1));
        rec_hdr.content_length = nyx_swap32(rec_content_bytes / 2);
        fwrite(&rec_hdr, sizeof(NyxShpRecordHeader), 1, fp);

        uint32_t st = f->shape_type;
        fwrite(&st, 4, 1, fp);

        if (f->shape_type == NYX_SHAPE_POINT) {
            fwrite(&f->points[0].x, 8, 1, fp);
            fwrite(&f->points[0].y, 8, 1, fp);
        } else if (f->shape_type == NYX_SHAPE_POLYGON || f->shape_type == NYX_SHAPE_POLYLINE) {
            fwrite(&f->x_min, 8, 1, fp);
            fwrite(&f->y_min, 8, 1, fp);
            fwrite(&f->x_max, 8, 1, fp);
            fwrite(&f->y_max, 8, 1, fp);
            int32_t np = f->num_parts;
            int32_t npts = f->num_points;
            fwrite(&np, 4, 1, fp);
            fwrite(&npts, 4, 1, fp);
            fwrite(f->parts, 4, np, fp);
            for (int p = 0; p < npts; p++) {
                fwrite(&f->points[p].x, 8, 1, fp);
                fwrite(&f->points[p].y, 8, 1, fp);
            }
        }
    }

    fclose(fp);
    return true;
}

/* Parse an ESRI Shapefile from disk */
static inline bool nyx_shp_read(const char* filepath, NyxShpDataset* out_ds) {
    FILE* fp = fopen(filepath, "rb");
    if (!fp) return false;

    memset(out_ds, 0, sizeof(NyxShpDataset));

    NyxShpHeader raw_hdr;
    if (fread(&raw_hdr, sizeof(NyxShpHeader), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    uint32_t code = nyx_swap32(raw_hdr.file_code);
    if (code != NYX_SHP_FILE_CODE) {
        fclose(fp);
        return false;
    }

    out_ds->header = raw_hdr;
    out_ds->header.file_code = code;
    out_ds->header.file_length = nyx_swap32(raw_hdr.file_length) * 2; /* convert to bytes */

    /* Read sequential records */
    while (out_ds->feature_count < NYX_MAX_SHP_FEATURES) {
        NyxShpRecordHeader rec_hdr;
        if (fread(&rec_hdr, sizeof(NyxShpRecordHeader), 1, fp) != 1) {
            break; /* EOF */
        }

        uint32_t rec_bytes = nyx_swap32(rec_hdr.content_length) * 2;
        NyxShpFeature* f = &out_ds->features[out_ds->feature_count];
        f->record_id = (int32_t)nyx_swap32(rec_hdr.record_number);

        uint32_t shape_type = 0;
        if (fread(&shape_type, 4, 1, fp) != 1) break;
        f->shape_type = (int32_t)shape_type;

        if (shape_type == NYX_SHAPE_POINT) {
            if (fread(&f->points[0].x, 8, 1, fp) != 1) break;
            if (fread(&f->points[0].y, 8, 1, fp) != 1) break;
            f->num_points = 1;
            f->num_parts = 1;
            f->parts[0] = 0;
            f->x_min = f->x_max = f->points[0].x;
            f->y_min = f->y_max = f->points[0].y;
            out_ds->feature_count++;
        } else if (shape_type == NYX_SHAPE_POLYGON || shape_type == NYX_SHAPE_POLYLINE) {
            if (fread(&f->x_min, 8, 1, fp) != 1) break;
            if (fread(&f->y_min, 8, 1, fp) != 1) break;
            if (fread(&f->x_max, 8, 1, fp) != 1) break;
            if (fread(&f->y_max, 8, 1, fp) != 1) break;

            int32_t num_parts = 0, num_points = 0;
            if (fread(&num_parts, 4, 1, fp) != 1) break;
            if (fread(&num_points, 4, 1, fp) != 1) break;

            f->num_parts = num_parts > 8 ? 8 : num_parts;
            f->num_points = num_points > NYX_MAX_SHP_VERTICES ? NYX_MAX_SHP_VERTICES : num_points;

            if (fread(f->parts, 4, (size_t)f->num_parts, fp) != (size_t)f->num_parts) break;

            for (int p = 0; p < f->num_points; p++) {
                if (fread(&f->points[p].x, 8, 1, fp) != 1) break;
                if (fread(&f->points[p].y, 8, 1, fp) != 1) break;
            }

            /* Seek past any unread overflow points in this record */
            if (num_points > NYX_MAX_SHP_VERTICES) {
                fseek(fp, (num_points - NYX_MAX_SHP_VERTICES) * 16, SEEK_CUR);
            }

            out_ds->feature_count++;
        } else {
            /* Skip unsupported shape records */
            if (rec_bytes > 4) {
                fseek(fp, rec_bytes - 4, SEEK_CUR);
            }
        }
    }

    fclose(fp);
    return true;
}

/* ── 3. GeoTIFF (.tif) Binary Specification (TIFF 6.0 + GeoTIFF 1.0) ─────── */
#define NYX_TIFF_MAGIC_LE 0x4949  /* 'II' Intel Little Endian */
#define NYX_TIFF_VERSION  42

/* Baseline TIFF Tag IDs */
#define TIFF_TAG_IMAGE_WIDTH         256
#define TIFF_TAG_IMAGE_LENGTH        257
#define TIFF_TAG_BITS_PER_SAMPLE     258
#define TIFF_TAG_COMPRESSION         259
#define TIFF_TAG_PHOTOMETRIC         262
#define TIFF_TAG_STRIP_OFFSETS       273
#define TIFF_TAG_SAMPLES_PER_PIXEL   277
#define TIFF_TAG_ROWS_PER_STRIP      278
#define TIFF_TAG_STRIP_BYTE_COUNTS   279

/* GeoTIFF Specific Tags */
#define TIFF_TAG_MODEL_PIXEL_SCALE   33550 /* ScaleX, ScaleY, ScaleZ */
#define TIFF_TAG_MODEL_TIEPOINT      33922 /* I, J, K, X, Y, Z */
#define TIFF_TAG_GEO_KEY_DIRECTORY   34735 /* Key directory version, rev, values */

#pragma pack(push, 1)
typedef struct {
    uint16_t byte_order;   /* 'II' = 0x4949 */
    uint16_t magic;        /* 42 */
    uint32_t ifd_offset;   /* Offset to first IFD */
} NyxTiffHeader;

typedef struct {
    uint16_t tag_id;
    uint16_t data_type;    /* 1=BYTE, 2=ASCII, 3=SHORT, 4=LONG, 5=RATIONAL, 12=DOUBLE */
    uint32_t count;
    uint32_t value_offset; /* If fits in 4 bytes, stored directly; else file offset */
} NyxTiffEntry;
#pragma pack(pop)

#define NYX_MAX_GEOTIFF_DIM 256

typedef struct {
    uint32_t width;
    uint32_t height;
    uint16_t bits_per_sample;
    uint16_t samples_per_pixel;
    uint16_t compression;
    /* Georeferencing model */
    double   pixel_scale_x;
    double   pixel_scale_y;
    double   tiepoint_i;
    double   tiepoint_j;
    double   tiepoint_x;   /* World origin X (e.g. Min Lon or Westing) */
    double   tiepoint_y;   /* World origin Y (e.g. Max Lat or Northing) */
    uint32_t epsg_code;    /* e.g., 4326 or 3857 */
    /* Raster Elevation / Pixel Values (Float array) */
    float    pixels[NYX_MAX_GEOTIFF_DIM * NYX_MAX_GEOTIFF_DIM];
} NyxGeoTiff;

/* Write a clean georeferenced GeoTIFF (Uncompressed elevation DEM) to disk */
static inline bool nyx_geotiff_write(const char* filepath, const NyxGeoTiff* gt) {
    FILE* fp = fopen(filepath, "wb");
    if (!fp) return false;

    NyxTiffHeader hdr;
    hdr.byte_order = NYX_TIFF_MAGIC_LE;
    hdr.magic = NYX_TIFF_VERSION;
    hdr.ifd_offset = 8; /* IFD starts immediately after 8-byte header */
    fwrite(&hdr, sizeof(NyxTiffHeader), 1, fp);

    uint16_t num_entries = 12;
    fwrite(&num_entries, 2, 1, fp);

    /* Pixel data starts after header + IFD + entries + offset arrays */
    uint32_t pixel_data_offset = 512;
    uint32_t pixel_bytes = gt->width * gt->height * (gt->bits_per_sample / 8);

    uint32_t scale_offset = 300;
    uint32_t tiepoint_offset = 330;
    uint32_t geokey_offset = 380;

    NyxTiffEntry entries[12] = {
        {TIFF_TAG_IMAGE_WIDTH,        4, 1, gt->width},
        {TIFF_TAG_IMAGE_LENGTH,       4, 1, gt->height},
        {TIFF_TAG_BITS_PER_SAMPLE,    3, 1, gt->bits_per_sample},
        {TIFF_TAG_COMPRESSION,        3, 1, 1}, /* 1 = Uncompressed */
        {TIFF_TAG_PHOTOMETRIC,        3, 1, 1}, /* 1 = BlackIsZero */
        {TIFF_TAG_STRIP_OFFSETS,      4, 1, pixel_data_offset},
        {TIFF_TAG_SAMPLES_PER_PIXEL,  3, 1, gt->samples_per_pixel},
        {TIFF_TAG_ROWS_PER_STRIP,     4, 1, gt->height},
        {TIFF_TAG_STRIP_BYTE_COUNTS,  4, 1, pixel_bytes},
        {TIFF_TAG_MODEL_PIXEL_SCALE, 12, 3, scale_offset},
        {TIFF_TAG_MODEL_TIEPOINT,    12, 6, tiepoint_offset},
        {TIFF_TAG_GEO_KEY_DIRECTORY,  3, 8, geokey_offset}
    };
    fwrite(entries, sizeof(NyxTiffEntry), num_entries, fp);

    uint32_t next_ifd = 0;
    fwrite(&next_ifd, 4, 1, fp);

    /* Write Pixel Scale (ScaleX, ScaleY, ScaleZ) at scale_offset */
    fseek(fp, scale_offset, SEEK_SET);
    double scales[3] = {gt->pixel_scale_x, gt->pixel_scale_y, 0.0};
    fwrite(scales, sizeof(double), 3, fp);

    /* Write Model Tiepoints (I, J, K, X, Y, Z) at tiepoint_offset */
    fseek(fp, tiepoint_offset, SEEK_SET);
    double tiepoints[6] = {gt->tiepoint_i, gt->tiepoint_j, 0.0, gt->tiepoint_x, gt->tiepoint_y, 0.0};
    fwrite(tiepoints, sizeof(double), 6, fp);

    /* Write GeoKeyDirectory (Header: 1, 1, 0, 1 + Key: GTModelTypeGeoKey, 0, 1, 1) */
    fseek(fp, geokey_offset, SEEK_SET);
    uint16_t geokeys[8] = {1, 1, 0, 1, 1024, 0, 1, 2}; /* ModelType 2 = Geographic 2D */
    fwrite(geokeys, sizeof(uint16_t), 8, fp);

    /* Write Pixel Data at pixel_data_offset */
    fseek(fp, pixel_data_offset, SEEK_SET);
    if (gt->bits_per_sample == 32) {
        fwrite(gt->pixels, sizeof(float), gt->width * gt->height, fp);
    } else {
        /* Convert float to uint8 for 8-bit output */
        uint8_t* u8 = (uint8_t*)malloc(gt->width * gt->height);
        for (uint32_t p = 0; p < gt->width * gt->height; p++) {
            float v = gt->pixels[p];
            if (v < 0.0f) v = 0.0f;
            if (v > 255.0f) v = 255.0f;
            u8[p] = (uint8_t)v;
        }
        fwrite(u8, 1, gt->width * gt->height, fp);
        free(u8);
    }

    fclose(fp);
    return true;
}

/* Parse GeoTIFF metadata, georeference tags, and raster elevation values from disk */
static inline bool nyx_geotiff_read(const char* filepath, NyxGeoTiff* out_gt) {
    FILE* fp = fopen(filepath, "rb");
    if (!fp) return false;

    memset(out_gt, 0, sizeof(NyxGeoTiff));

    NyxTiffHeader hdr;
    if (fread(&hdr, sizeof(NyxTiffHeader), 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    if (hdr.byte_order != NYX_TIFF_MAGIC_LE || hdr.magic != NYX_TIFF_VERSION) {
        fclose(fp);
        return false;
    }

    fseek(fp, hdr.ifd_offset, SEEK_SET);
    uint16_t num_entries = 0;
    if (fread(&num_entries, 2, 1, fp) != 1) {
        fclose(fp);
        return false;
    }

    uint32_t strip_offset = 0;
    uint32_t strip_bytes = 0;

    for (uint16_t e = 0; e < num_entries; e++) {
        NyxTiffEntry entry;
        if (fread(&entry, sizeof(NyxTiffEntry), 1, fp) != 1) break;

        switch (entry.tag_id) {
            case TIFF_TAG_IMAGE_WIDTH:
                out_gt->width = entry.value_offset;
                break;
            case TIFF_TAG_IMAGE_LENGTH:
                out_gt->height = entry.value_offset;
                break;
            case TIFF_TAG_BITS_PER_SAMPLE:
                out_gt->bits_per_sample = (uint16_t)entry.value_offset;
                break;
            case TIFF_TAG_SAMPLES_PER_PIXEL:
                out_gt->samples_per_pixel = (uint16_t)entry.value_offset;
                break;
            case TIFF_TAG_COMPRESSION:
                out_gt->compression = (uint16_t)entry.value_offset;
                break;
            case TIFF_TAG_STRIP_OFFSETS:
                strip_offset = entry.value_offset;
                break;
            case TIFF_TAG_STRIP_BYTE_COUNTS:
                strip_bytes = entry.value_offset;
                break;
            case TIFF_TAG_MODEL_PIXEL_SCALE: {
                long cur = ftell(fp);
                fseek(fp, entry.value_offset, SEEK_SET);
                double s[3];
                if (fread(s, sizeof(double), 3, fp) == 3) {
                    out_gt->pixel_scale_x = s[0];
                    out_gt->pixel_scale_y = s[1];
                }
                fseek(fp, cur, SEEK_SET);
                break;
            }
            case TIFF_TAG_MODEL_TIEPOINT: {
                long cur = ftell(fp);
                fseek(fp, entry.value_offset, SEEK_SET);
                double t[6];
                if (fread(t, sizeof(double), 6, fp) == 6) {
                    out_gt->tiepoint_i = t[0];
                    out_gt->tiepoint_j = t[1];
                    out_gt->tiepoint_x = t[3];
                    out_gt->tiepoint_y = t[4];
                }
                fseek(fp, cur, SEEK_SET);
                break;
            }
            case TIFF_TAG_GEO_KEY_DIRECTORY:
                out_gt->epsg_code = 4326; /* Default WGS-84 */
                break;
            default:
                break;
        }
    }

    if (out_gt->bits_per_sample == 0) out_gt->bits_per_sample = 8;
    if (out_gt->samples_per_pixel == 0) out_gt->samples_per_pixel = 1;

    /* Read Raster Pixels */
    if (strip_offset > 0 && out_gt->width > 0 && out_gt->height > 0) {
        fseek(fp, strip_offset, SEEK_SET);
        size_t total_px = out_gt->width * out_gt->height;
        if (total_px > NYX_MAX_GEOTIFF_DIM * NYX_MAX_GEOTIFF_DIM) {
            total_px = NYX_MAX_GEOTIFF_DIM * NYX_MAX_GEOTIFF_DIM;
        }

        if (out_gt->bits_per_sample == 32) {
            fread(out_gt->pixels, sizeof(float), total_px, fp);
        } else if (out_gt->bits_per_sample == 8) {
            uint8_t* raw = (uint8_t*)malloc(total_px);
            if (raw) {
                fread(raw, 1, total_px, fp);
                for (size_t i = 0; i < total_px; i++) {
                    out_gt->pixels[i] = (float)raw[i];
                }
                free(raw);
            }
        }
    }

    fclose(fp);
    return true;
}

#ifdef __cplusplus
}
#endif

#endif /* RT_GIS_FORMATS_H */
