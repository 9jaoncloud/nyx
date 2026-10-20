/*
 * rt_gis_engine.c — Nyx Runtime Enterprise GIS & Mapping Engine Implementation
 */

#include "rt_gis_engine.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PI 3.14159265358979323846
#define RADIUS_EARTH_METERS 6378137.0

/* ── 1. Bounding Box Math ────────────────────────────────────────────────── */
bool nyx_bbox_intersects(NyxBBox a, NyxBBox b) {
    return !(a.max_x < b.min_x || a.min_x > b.max_x || a.max_y < b.min_y || a.min_y > b.max_y);
}

bool nyx_bbox_contains_point(NyxBBox box, NyxGeoPoint pt) {
    return (pt.x >= box.min_x && pt.x <= box.max_x && pt.y >= box.min_y && pt.y <= box.max_y);
}

/* ── 2. Ramer-Douglas-Peucker (RDP) Algorithm ────────────────────────────── */
static double perpendicular_distance(NyxGeoPoint pt, NyxGeoPoint line_start, NyxGeoPoint line_end) {
    double dx = line_end.x - line_start.x;
    double dy = line_end.y - line_start.y;
    double line_len_sq = dx * dx + dy * dy;

    if (line_len_sq == 0.0) {
        double px = pt.x - line_start.x;
        double py = pt.y - line_start.y;
        return sqrt(px * px + py * py);
    }

    double t = ((pt.x - line_start.x) * dx + (pt.y - line_start.y) * dy) / line_len_sq;
    if (t < 0.0) t = 0.0;
    else if (t > 1.0) t = 1.0;

    double proj_x = line_start.x + t * dx;
    double proj_y = line_start.y + t * dy;

    double dist_x = pt.x - proj_x;
    double dist_y = pt.y - proj_y;
    return sqrt(dist_x * dist_x + dist_y * dist_y);
}

static void rdp_recursive(const NyxGeoPoint* in_pts, size_t start, size_t end, double epsilon, bool* keep_mask) {
    if (end <= start + 1) return;

    double max_dist = 0.0;
    size_t max_idx = start;

    for (size_t i = start + 1; i < end; i++) {
        double dist = perpendicular_distance(in_pts[i], in_pts[start], in_pts[end]);
        if (dist > max_dist) {
            max_dist = dist;
            max_idx = i;
        }
    }

    if (max_dist > epsilon) {
        keep_mask[max_idx] = true;
        rdp_recursive(in_pts, start, max_idx, epsilon, keep_mask);
        rdp_recursive(in_pts, max_idx, end, epsilon, keep_mask);
    }
}

size_t nyx_gis_simplify_rdp(const NyxGeoPoint* in_pts, size_t count, double epsilon, NyxGeoPoint* out_pts, size_t max_out) {
    if (!in_pts || count == 0 || !out_pts || max_out == 0) return 0;
    if (count <= 2) {
        size_t n = (count < max_out) ? count : max_out;
        memcpy(out_pts, in_pts, sizeof(NyxGeoPoint) * n);
        return n;
    }

    bool* keep_mask = (bool*)calloc(count, sizeof(bool));
    keep_mask[0] = true;
    keep_mask[count - 1] = true;

    rdp_recursive(in_pts, 0, count - 1, epsilon, keep_mask);

    size_t out_count = 0;
    for (size_t i = 0; i < count; i++) {
        if (keep_mask[i] && out_count < max_out) {
            out_pts[out_count++] = in_pts[i];
        }
    }

    free(keep_mask);
    return out_count;
}

/* ── 3. Point-in-Polygon (Ray-Casting Algorithm) ─────────────────────────── */
bool nyx_gis_point_in_polygon(NyxGeoPoint pt, const NyxGeoPoint* poly, size_t count) {
    if (!poly || count < 3) return false;

    bool inside = false;
    for (size_t i = 0, j = count - 1; i < count; j = i++) {
        if (((poly[i].y > pt.y) != (poly[j].y > pt.y)) &&
            (pt.x < (poly[j].x - poly[i].x) * (pt.y - poly[i].y) / (poly[j].y - poly[i].y) + poly[i].x)) {
            inside = !inside;
        }
    }
    return inside;
}

/* ── 4. Web Mercator (EPSG:3857) & Slippy Tile Math ──────────────────────── */
void nyx_gis_wgs84_to_web_mercator(double lon_deg, double lat_deg, double* out_x, double* out_y) {
    if (out_x) *out_x = lon_deg * (2.0 * PI * RADIUS_EARTH_METERS / 360.0);
    if (out_y) {
        if (lat_deg > 85.05112878) lat_deg = 85.05112878;
        if (lat_deg < -85.05112878) lat_deg = -85.05112878;
        double lat_rad = lat_deg * (PI / 180.0);
        *out_y = log(tan((PI / 4.0) + (lat_rad / 2.0))) * RADIUS_EARTH_METERS;
    }
}

void nyx_gis_web_mercator_to_wgs84(double x, double y, double* out_lon, double* out_lat) {
    if (out_lon) *out_lon = (x / RADIUS_EARTH_METERS) * (180.0 / PI);
    if (out_lat) *out_lat = (2.0 * atan(exp(y / RADIUS_EARTH_METERS)) - (PI / 2.0)) * (180.0 / PI);
}

void nyx_gis_latlon_to_tile(double lon_deg, double lat_deg, int zoom, int* out_tile_x, int* out_tile_y) {
    double n = pow(2.0, zoom);
    if (out_tile_x) *out_tile_x = (int)((lon_deg + 180.0) / 360.0 * n);
    if (out_tile_y) {
        double lat_rad = lat_deg * (PI / 180.0);
        *out_tile_y = (int)((1.0 - asinh(tan(lat_rad)) / PI) / 2.0 * n);
    }
}

/* ── 5. A* Network Graph Router ──────────────────────────────────────────── */
static double heuristic_dist(NyxGeoPoint a, NyxGeoPoint b) {
    double dx = a.x - b.x;
    double dy = a.y - b.y;
    return sqrt(dx * dx + dy * dy);
}

double nyx_gis_astar_route(const NyxRoadGraph* graph, int start, int target, int* out_path, size_t* out_path_len) {
    if (!graph || start < 0 || target < 0 || (size_t)start >= graph->node_count || (size_t)target >= graph->node_count) {
        return -1.0;
    }

    double g_score[32];
    double f_score[32];
    int came_from[32];
    bool closed_set[32];

    for (size_t i = 0; i < 32; i++) {
        g_score[i] = 1e9;
        f_score[i] = 1e9;
        came_from[i] = -1;
        closed_set[i] = false;
    }

    g_score[start] = 0.0;
    f_score[start] = heuristic_dist(graph->node_coords[start], graph->node_coords[target]);

    while (true) {
        int current = -1;
        double lowest_f = 1e9;

        for (size_t i = 0; i < graph->node_count; i++) {
            if (!closed_set[i] && f_score[i] < lowest_f) {
                lowest_f = f_score[i];
                current = (int)i;
            }
        }

        if (current == -1) return -1.0; /* No path found */
        if (current == target) {
            /* Reconstruct path */
            size_t count = 0;
            int curr = target;
            int temp_path[32];
            while (curr != -1) {
                temp_path[count++] = curr;
                curr = came_from[curr];
            }
            if (out_path && out_path_len) {
                for (size_t i = 0; i < count; i++) {
                    out_path[i] = temp_path[count - 1 - i];
                }
                *out_path_len = count;
            }
            return g_score[target];
        }

        closed_set[current] = true;

        for (size_t e = 0; e < graph->edge_count; e++) {
            if (graph->edges[e].u == current) {
                int neighbor = graph->edges[e].v;
                if (closed_set[neighbor]) continue;

                double tentative_g = g_score[current] + graph->edges[e].weight;
                if (tentative_g < g_score[neighbor]) {
                    came_from[neighbor] = current;
                    g_score[neighbor] = tentative_g;
                    f_score[neighbor] = tentative_g + heuristic_dist(graph->node_coords[neighbor], graph->node_coords[target]);
                }
            }
        }
    }
}
