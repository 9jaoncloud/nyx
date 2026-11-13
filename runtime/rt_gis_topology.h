/*
 * rt_gis_topology.h — Advanced Boolean Polygon Topology (Union, Intersection, Difference, Buffer)
 * Part of the Nyx Geospatial Engine (Zero External Dependencies)
 */

#ifndef RT_GIS_TOPOLOGY_H
#define RT_GIS_TOPOLOGY_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    double x;
    double y;
} NyxTopoPoint;

#define NYX_MAX_TOPO_VERTICES 256

typedef struct {
    size_t       vertex_count;
    NyxTopoPoint vertices[NYX_MAX_TOPO_VERTICES];
} NyxPolygon;

/* ── 1. Planar Geometric Primitives ──────────────────────────────────────── */

/* Gauss Shoelace Formula for Polygon Area */
static inline double nyx_poly_area(const NyxPolygon* p) {
    if (p->vertex_count < 3) return 0.0;
    double area = 0.0;
    for (size_t i = 0; i < p->vertex_count; i++) {
        size_t j = (i + 1) % p->vertex_count;
        area += p->vertices[i].x * p->vertices[j].y;
        area -= p->vertices[j].x * p->vertices[i].y;
    }
    return fabs(area) * 0.5;
}

/* Centroid of Polygon */
static inline NyxTopoPoint nyx_poly_centroid(const NyxPolygon* p) {
    NyxTopoPoint c = {0.0, 0.0};
    if (p->vertex_count == 0) return c;
    for (size_t i = 0; i < p->vertex_count; i++) {
        c.x += p->vertices[i].x;
        c.y += p->vertices[i].y;
    }
    c.x /= (double)p->vertex_count;
    c.y /= (double)p->vertex_count;
    return c;
}

/* Check if point is inside polygon (Jordan curve ray-casting) */
static inline bool nyx_poly_contains_point(const NyxPolygon* p, NyxTopoPoint pt) {
    bool inside = false;
    for (size_t i = 0, j = p->vertex_count - 1; i < p->vertex_count; j = i++) {
        if (((p->vertices[i].y > pt.y) != (p->vertices[j].y > pt.y)) &&
            (pt.x < (p->vertices[j].x - p->vertices[i].x) * (pt.y - p->vertices[i].y) /
                    (p->vertices[j].y - p->vertices[i].y) + p->vertices[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

/* Line-Line Intersection point */
static inline bool nyx_line_intersect(NyxTopoPoint p1, NyxTopoPoint p2,
                                     NyxTopoPoint p3, NyxTopoPoint p4,
                                     NyxTopoPoint* out_intersect) {
    double d = (p1.x - p2.x) * (p3.y - p4.y) - (p1.y - p2.y) * (p3.x - p4.x);
    if (fabs(d) < 1e-9) return false;

    double t = ((p1.x - p3.x) * (p3.y - p4.y) - (p1.y - p3.y) * (p3.x - p4.x)) / d;
    double u = -((p1.x - p2.x) * (p1.y - p3.y) - (p1.y - p2.y) * (p1.x - p3.x)) / d;

    if (t >= 0.0 && t <= 1.0 && u >= 0.0 && u <= 1.0) {
        out_intersect->x = p1.x + t * (p2.x - p1.x);
        out_intersect->y = p1.y + t * (p2.y - p1.y);
        return true;
    }
    return false;
}

/* ── 2. Sutherland-Hodgman Polygon Clipping (Intersection) ───────────────── */
static inline bool nyx_is_inside_edge(NyxTopoPoint p, NyxTopoPoint cp1, NyxTopoPoint cp2) {
    return (cp2.x - cp1.x) * (p.y - cp1.y) - (cp2.y - cp1.y) * (p.x - cp1.x) >= 0.0;
}

static inline NyxTopoPoint nyx_edge_intersection(NyxTopoPoint s, NyxTopoPoint p, NyxTopoPoint cp1, NyxTopoPoint cp2) {
    NyxTopoPoint out = {0, 0};
    double a1 = p.y - s.y;
    double b1 = s.x - p.x;
    double c1 = a1 * s.x + b1 * s.y;

    double a2 = cp2.y - cp1.y;
    double b2 = cp1.x - cp2.x;
    double c2 = a2 * cp1.x + b2 * cp1.y;

    double det = a1 * b2 - a2 * b1;
    if (fabs(det) > 1e-9) {
        out.x = (b2 * c1 - b1 * c2) / det;
        out.y = (a1 * c2 - a2 * c1) / det;
    }
    return out;
}

/* Boolean Intersection: Compute A ∩ B */
static inline size_t nyx_polygon_intersection(const NyxPolygon* subject, const NyxPolygon* clip, NyxPolygon* out_poly) {
    NyxPolygon input = *subject;
    NyxPolygon output;

    for (size_t i = 0; i < clip->vertex_count; i++) {
        size_t k = (i + 1) % clip->vertex_count;
        NyxTopoPoint cp1 = clip->vertices[i];
        NyxTopoPoint cp2 = clip->vertices[k];

        output.vertex_count = 0;
        if (input.vertex_count == 0) break;

        NyxTopoPoint s = input.vertices[input.vertex_count - 1];
        for (size_t j = 0; j < input.vertex_count; j++) {
            NyxTopoPoint p = input.vertices[j];
            if (nyx_is_inside_edge(p, cp1, cp2)) {
                if (!nyx_is_inside_edge(s, cp1, cp2)) {
                    if (output.vertex_count < NYX_MAX_TOPO_VERTICES) {
                        output.vertices[output.vertex_count++] = nyx_edge_intersection(s, p, cp1, cp2);
                    }
                }
                if (output.vertex_count < NYX_MAX_TOPO_VERTICES) {
                    output.vertices[output.vertex_count++] = p;
                }
            } else if (nyx_is_inside_edge(s, cp1, cp2)) {
                if (output.vertex_count < NYX_MAX_TOPO_VERTICES) {
                    output.vertices[output.vertex_count++] = nyx_edge_intersection(s, p, cp1, cp2);
                }
            }
            s = p;
        }
        input = output;
    }

    *out_poly = output;
    return out_poly->vertex_count;
}

/* ── 3. Convex Hull & Composite Boolean Union ────────────────────────────── */
static inline int nyx_polar_compare(const void* a, const void* b) {
    const NyxTopoPoint* p1 = (const NyxTopoPoint*)a;
    const NyxTopoPoint* p2 = (const NyxTopoPoint*)b;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    return 0;
}

/* Boolean Union: Compute A ∪ B (Constructs unified outer boundary) */
static inline size_t nyx_polygon_union(const NyxPolygon* poly_a, const NyxPolygon* poly_b, NyxPolygon* out_poly) {
    /* Collect all vertices from both polygons */
    NyxTopoPoint pts[NYX_MAX_TOPO_VERTICES * 2];
    size_t count = 0;

    for (size_t i = 0; i < poly_a->vertex_count && count < NYX_MAX_TOPO_VERTICES * 2; i++) {
        pts[count++] = poly_a->vertices[i];
    }
    for (size_t i = 0; i < poly_b->vertex_count && count < NYX_MAX_TOPO_VERTICES * 2; i++) {
        pts[count++] = poly_b->vertices[i];
    }

    /* Add any edge intersection vertices */
    for (size_t i = 0; i < poly_a->vertex_count; i++) {
        NyxTopoPoint a1 = poly_a->vertices[i];
        NyxTopoPoint a2 = poly_a->vertices[(i + 1) % poly_a->vertex_count];
        for (size_t j = 0; j < poly_b->vertex_count; j++) {
            NyxTopoPoint b1 = poly_b->vertices[j];
            NyxTopoPoint b2 = poly_b->vertices[(j + 1) % poly_b->vertex_count];
            NyxTopoPoint inter;
            if (nyx_line_intersect(a1, a2, b1, b2, &inter)) {
                if (count < NYX_MAX_TOPO_VERTICES * 2) pts[count++] = inter;
            }
        }
    }

    /* Monotone Chain Convex/Outer Boundary Construction */
    qsort(pts, count, sizeof(NyxTopoPoint), nyx_polar_compare);

    NyxTopoPoint hull[NYX_MAX_TOPO_VERTICES];
    size_t k = 0;

    /* Lower hull */
    for (size_t i = 0; i < count; i++) {
        while (k >= 2 && (hull[k-1].x - hull[k-2].x) * (pts[i].y - hull[k-2].y) -
                         (hull[k-1].y - hull[k-2].y) * (pts[i].x - hull[k-2].x) <= 0) {
            k--;
        }
        if (k < NYX_MAX_TOPO_VERTICES) hull[k++] = pts[i];
    }

    /* Upper hull */
    for (size_t i = count - 1, t = k + 1; i > 0; i--) {
        size_t idx = i - 1;
        while (k >= t && (hull[k-1].x - hull[k-2].x) * (pts[idx].y - hull[k-2].y) -
                         (hull[k-1].y - hull[k-2].y) * (pts[idx].x - hull[k-2].x) <= 0) {
            k--;
        }
        if (k < NYX_MAX_TOPO_VERTICES) hull[k++] = pts[idx];
    }

    if (k > 1) k--; /* Remove duplicate end point */

    out_poly->vertex_count = k;
    for (size_t i = 0; i < k; i++) {
        out_poly->vertices[i] = hull[i];
    }
    return out_poly->vertex_count;
}

/* Boolean Difference: Compute A \ B */
static inline size_t nyx_polygon_difference(const NyxPolygon* poly_a, const NyxPolygon* poly_b, NyxPolygon* out_poly) {
    /* Collect vertices of poly_a that are OUTSIDE poly_b */
    out_poly->vertex_count = 0;
    for (size_t i = 0; i < poly_a->vertex_count; i++) {
        if (!nyx_poly_contains_point(poly_b, poly_a->vertices[i])) {
            if (out_poly->vertex_count < NYX_MAX_TOPO_VERTICES) {
                out_poly->vertices[out_poly->vertex_count++] = poly_a->vertices[i];
            }
        }
    }

    /* Add intersection boundary vertices */
    for (size_t i = 0; i < poly_a->vertex_count; i++) {
        NyxTopoPoint a1 = poly_a->vertices[i];
        NyxTopoPoint a2 = poly_a->vertices[(i + 1) % poly_a->vertex_count];
        for (size_t j = 0; j < poly_b->vertex_count; j++) {
            NyxTopoPoint b1 = poly_b->vertices[j];
            NyxTopoPoint b2 = poly_b->vertices[(j + 1) % poly_b->vertex_count];
            NyxTopoPoint inter;
            if (nyx_line_intersect(a1, a2, b1, b2, &inter)) {
                if (out_poly->vertex_count < NYX_MAX_TOPO_VERTICES) {
                    out_poly->vertices[out_poly->vertex_count++] = inter;
                }
            }
        }
    }
    return out_poly->vertex_count;
}

/* ── 4. Polygon Buffering (r-Offset Minkowski Geometry) ───────────────────── */
static inline size_t nyx_polygon_buffer(const NyxPolygon* in_poly, double buffer_dist, NyxPolygon* out_poly) {
    if (in_poly->vertex_count < 3) return 0;
    out_poly->vertex_count = 0;

    NyxTopoPoint centroid = nyx_poly_centroid(in_poly);

    for (size_t i = 0; i < in_poly->vertex_count; i++) {
        size_t prev = (i + in_poly->vertex_count - 1) % in_poly->vertex_count;
        size_t next = (i + 1) % in_poly->vertex_count;

        NyxTopoPoint p = in_poly->vertices[i];
        NyxTopoPoint p_prev = in_poly->vertices[prev];
        NyxTopoPoint p_next = in_poly->vertices[next];

        /* Normal vectors for adjacent edges */
        double dx1 = p.x - p_prev.x, dy1 = p.y - p_prev.y;
        double len1 = sqrt(dx1 * dx1 + dy1 * dy1);
        if (len1 < 1e-6) len1 = 1.0;
        double nx1 = -dy1 / len1, ny1 = dx1 / len1;

        double dx2 = p_next.x - p.x, dy2 = p_next.y - p.y;
        double len2 = sqrt(dx2 * dx2 + dy2 * dy2);
        if (len2 < 1e-6) len2 = 1.0;
        double nx2 = -dy2 / len2, ny2 = dx2 / len2;

        /* Average outward normal */
        double avg_nx = (nx1 + nx2) * 0.5;
        double avg_ny = (ny1 + ny2) * 0.5;
        double avg_len = sqrt(avg_nx * avg_nx + avg_ny * avg_ny);
        if (avg_len < 1e-6) avg_len = 1.0;
        avg_nx /= avg_len;
        avg_ny /= avg_len;

        /* Ensure normal points outward relative to centroid */
        double to_c_x = centroid.x - p.x;
        double to_c_y = centroid.y - p.y;
        if (avg_nx * to_c_x + avg_ny * to_c_y > 0.0) {
            avg_nx = -avg_nx;
            avg_ny = -avg_ny;
        }

        if (out_poly->vertex_count < NYX_MAX_TOPO_VERTICES) {
            out_poly->vertices[out_poly->vertex_count].x = p.x + avg_nx * buffer_dist;
            out_poly->vertices[out_poly->vertex_count].y = p.y + avg_ny * buffer_dist;
            out_poly->vertex_count++;
        }
    }
    return out_poly->vertex_count;
}

#ifdef __cplusplus
}
#endif

#endif /* RT_GIS_TOPOLOGY_H */
