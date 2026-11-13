#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "runtime/rt_arena.h"
#include "runtime/rt_string.h"
#include "runtime/rt_io.h"
#include "runtime/rt_vec.h"
#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_net.h"
#include "runtime/rt_crypto.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_time.h"
#include "runtime/rt_nursery.h"
#include "runtime/rt_collections.h"
#include "runtime/rt_math.h"
#include "runtime/rt_format.h"
#include "runtime/rt_thread.h"
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif
#ifndef NYX_TUPLE_DEFS
#define NYX_TUPLE_DEFS
typedef struct { void* f0; void* f1; } NyxTuple2;
typedef struct { void* f0; void* f1; void* f2; } NyxTuple3;
typedef struct { void* f0; void* f1; void* f2; void* f3; } NyxTuple4;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; } NyxTuple5;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; } NyxTuple6;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; } NyxTuple7;
typedef struct { void* f0; void* f1; void* f2; void* f3; void* f4; void* f5; void* f6; void* f7; } NyxTuple8;
#endif

#define TAG_QmlNodeType_Root 0
#define TAG_QmlNodeType_Item 1
#define TAG_QmlNodeType_Rectangle 2
#define TAG_QmlNodeType_Text 3
#define TAG_QmlNodeType_Button 4
#define TAG_QmlNodeType_Column 5
#define TAG_QmlNodeType_Row 6
#define TAG_QmlNodeType_Grid 7
#define TAG_QmlNodeType_MouseArea 8
#define TAG_QmlNodeType_Animation 9
typedef struct {
    int tag;
    union {
    } data;
} QmlNodeType;

typedef struct {
    rt_string_t key;
    rt_string_t value_raw;
    double numeric_val;
} QmlProperty;

typedef struct {
    QmlNodeType node_type;
    rt_string_t type_name;
    rt_string_t id;
    double x;
    double y;
    double width;
    double height;
    double opacity;
    int visible;
    int64_t parent_index;
    int64_t child_count;
    NyxVec child_indices;
    int64_t property_count;
    NyxVec properties;
} QmlNode;

typedef struct {
    int64_t node_count;
    NyxVec nodes;
    int64_t root_index;
} QmlDocument;

NyxOption parse_qml_source(rt_string_t);
NyxVec compile_qml_to_material_scene(QmlDocument*);
// module qml_parser
// import prelude::*
// import collections::*
// import string::*
// import ui::material_ui::*
NyxOption parse_qml_source(rt_string_t qml_src) {
Arena* __arena = arena_create();
({ if (((qml_src).length == 0)) {
return rt_option_none();
}
});
NyxVec nodes = rt_vec_new(sizeof(int64_t));
QmlNode root = (QmlNode){.node_type = (QmlNodeType){ .tag = TAG_QmlNodeType_Rectangle }, .type_name = rt_string_from("Rectangle"), .id = rt_string_from("root"), .x = 0, .y = 0, .width = 1280, .height = 720, .opacity = 1, .visible = 1, .parent_index = 0LL, .child_count = 0LL, .child_indices = rt_vec_new(sizeof(int64_t)), .property_count = 0LL, .properties = rt_vec_new(sizeof(int64_t))};
rt_vec_push(&nodes, ({ QmlNode _tmp_1 = (int64_t)(root); &_tmp_1; }));
arena_destroy(__arena);
return ({ NyxOption _tmp; _tmp.tag = OPTION_SOME; _tmp.data.ptr_val = (void*)({ QmlDocument _stmp_ = (QmlDocument){.node_count = 1LL, .nodes = nodes, .root_index = 0LL}; &_stmp_; }); _tmp; });
}

NyxVec compile_qml_to_material_scene(QmlDocument* doc) {
Arena* __arena = arena_create();
NyxVec components = rt_vec_new(sizeof(MaterialComponent));
for (int64_t i = 0LL; i < (*doc).nodes.len; i++) {
int64_t node = (&((*(int64_t*)rt_vec_get((*doc).nodes, i))));
NyxVec comp_type = ({
ComponentType __match_result_2;
QmlNodeType __match_val_2 = node.node_type;
if (__match_val_2.tag == TAG_QmlNodeType_Button) {
__match_result_2 = (ComponentType){ .tag = TAG_ComponentType_ButtonFilled };
}
 else if (__match_val_2.tag == TAG_QmlNodeType_Rectangle) {
__match_result_2 = (ComponentType){ .tag = TAG_ComponentType_CardElevated };
}
 else {
__match_result_2 = (ComponentType){ .tag = TAG_ComponentType_CardOutlined };
}
__match_result_2;
});
NyxVec elevation = ({
MaterialElevation __match_result_3;
QmlNodeType __match_val_3 = node.node_type;
if (__match_val_3.tag == TAG_QmlNodeType_Button) {
__match_result_3 = (MaterialElevation){ .tag = TAG_MaterialElevation_Level1 };
}
 else if (__match_val_3.tag == TAG_QmlNodeType_Rectangle) {
__match_result_3 = (MaterialElevation){ .tag = TAG_MaterialElevation_Level1 };
}
 else {
__match_result_3 = (MaterialElevation){ .tag = TAG_MaterialElevation_Level0 };
}
__match_result_3;
});
NyxVec corner_radius = ({
MaterialCornerRadius __match_result_4;
QmlNodeType __match_val_4 = node.node_type;
if (__match_val_4.tag == TAG_QmlNodeType_Button) {
__match_result_4 = (MaterialCornerRadius){ .tag = TAG_MaterialCornerRadius_Full };
}
 else if (__match_val_4.tag == TAG_QmlNodeType_Rectangle) {
__match_result_4 = (MaterialCornerRadius){ .tag = TAG_MaterialCornerRadius_Medium };
}
 else {
__match_result_4 = (MaterialCornerRadius){ .tag = TAG_MaterialCornerRadius_Small };
}
__match_result_4;
});
rt_vec_push(&components, ({ MaterialComponent _tmp_5 = (int64_t)((MaterialComponent){.comp_type = comp_type, .x = node.x, .y = node.y, .width = node.width, .height = node.height, .elevation = elevation, .corner_radius = corner_radius, .is_hovered = 0, .is_pressed = 0, .is_focused = 0, .is_disabled = 0, .ripple_x = 0, .ripple_y = 0, .ripple_progress = 0, .label = rt_string_from(""), .state_value = 0}); &_tmp_5; }));
}
;
arena_destroy(__arena);
return components;
}

