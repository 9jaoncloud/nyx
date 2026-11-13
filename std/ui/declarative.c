#include "runtime/rt_option.h"
#include "runtime/rt_map.h"
#include "runtime/rt_async.h"
#include "runtime/rt_json.h"
#include "runtime/rt_string.h"
#include "runtime/rt_vec.h"
#include <stdint.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "runtime/rt_io.h"
#include "runtime/rt_db.h"
#include "runtime/rt_http.h"
#include "runtime/rt_concurrency.h"
#include "runtime/rt_stdlib.h"
#include "runtime/rt_arena.h"
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

#define TAG_WidgetKind_Container 0
#define TAG_WidgetKind_Column 1
#define TAG_WidgetKind_Row 2
#define TAG_WidgetKind_Text 3
#define TAG_WidgetKind_Button 4
#define TAG_WidgetKind_Card 5
#define TAG_WidgetKind_TextField 6
#define TAG_WidgetKind_Image 7
#define TAG_WidgetKind_Spacer 8
#define TAG_WidgetKind_RetinaCanvas 9
typedef struct {
    int tag;
    union {
    } data;
} WidgetKind;

typedef struct {
    double r;
    double g;
    double b;
    double a;
} Color;

typedef struct {
    double top;
    double right;
    double bottom;
    double left;
} EdgeInsets;

typedef struct {
    WidgetKind kind;
    rt_string_t key;
    rt_string_t text_content;
    EdgeInsets padding;
    EdgeInsets margin;
    Color background;
    double border_radius;
    double width;
    double height;
    int is_interactive;
    NyxVec children;
} WidgetNode;

typedef struct {
    int64_t frame_index;
    double delta_time_ms;
    double screen_width;
    double screen_height;
    double scale_factor;
} RenderFrameContext;

Color Color_hex(rt_string_t);
Color Color_rgba(double, double, double, double);
EdgeInsets EdgeInsets_all(double);
EdgeInsets EdgeInsets_symmetric(double, double);
WidgetNode WidgetNode_new(WidgetKind);
WidgetNode WidgetNode_text(rt_string_t, double, Color);
WidgetNode WidgetNode_button(rt_string_t, rt_string_t);
WidgetNode WidgetNode_column(NyxVec, double);
WidgetNode WidgetNode_row(NyxVec, double);
WidgetNode WidgetNode_card(WidgetNode, double);
RenderFrameContext RenderFrameContext_new(double, double, double);
int RenderFrameContext_render_tree(RenderFrameContext*, WidgetNode*);
// module std.ui.declarative
// import std::math
// import std::string
// import std::time
Color Color_hex(rt_string_t hex_code) {
return (Color){.r = 0.22, .g = 0.74, .b = 0.97, .a = 1};
}

Color Color_rgba(double red, double green, double blue, double alpha) {
return (Color){.r = red, .g = green, .b = blue, .a = alpha};
}

EdgeInsets EdgeInsets_all(double value) {
return (EdgeInsets){.top = value, .right = value, .bottom = value, .left = value};
}

EdgeInsets EdgeInsets_symmetric(double vertical, double horizontal) {
return (EdgeInsets){.top = vertical, .right = horizontal, .bottom = vertical, .left = horizontal};
}

WidgetNode WidgetNode_new(WidgetKind kind) {
return (WidgetNode){.kind = kind, .key = rt_string_from("node_0"), .text_content = rt_string_from(""), .padding = EdgeInsets_all(0), .margin = EdgeInsets_all(0), .background = Color_rgba(0, 0, 0, 0), .border_radius = 0, .width = (-1), .height = (-1), .is_interactive = 0, .children = rt_vec_new(sizeof(int64_t))};
}

WidgetNode WidgetNode_text(rt_string_t content, double size, Color color) {
WidgetNode node = WidgetNode_new((WidgetKind){ .tag = TAG_WidgetKind_Text });
(node.text_content = content);
(node.background = color);
(node.height = size);
return node;
}

WidgetNode WidgetNode_button(rt_string_t label, rt_string_t on_press_key) {
WidgetNode node = WidgetNode_new((WidgetKind){ .tag = TAG_WidgetKind_Button });
(node.text_content = label);
(node.key = on_press_key);
(node.is_interactive = 1);
(node.padding = EdgeInsets_symmetric(12, 24));
(node.border_radius = 8);
(node.background = Color_rgba(0.55, 0.36, 0.96, 1));
return node;
}

WidgetNode WidgetNode_column(NyxVec children, double spacing) {
WidgetNode node = WidgetNode_new((WidgetKind){ .tag = TAG_WidgetKind_Column });
(node.children = children);
(node.height = spacing);
return node;
}

WidgetNode WidgetNode_row(NyxVec children, double spacing) {
WidgetNode node = WidgetNode_new((WidgetKind){ .tag = TAG_WidgetKind_Row });
(node.children = children);
(node.width = spacing);
return node;
}

WidgetNode WidgetNode_card(WidgetNode child, double padding) {
WidgetNode node = WidgetNode_new((WidgetKind){ .tag = TAG_WidgetKind_Card });
(node.padding = EdgeInsets_all(padding));
(node.border_radius = 12);
(node.background = Color_rgba(0.05, 0.08, 0.16, 0.95));
rt_vec_push(&node.children, ({ WidgetNode _tmp_0 = child; &_tmp_0; }));
return node;
}

RenderFrameContext RenderFrameContext_new(double w, double h, double scale) {
return (RenderFrameContext){.frame_index = 0LL, .delta_time_ms = 8.33, .screen_width = w, .screen_height = h, .scale_factor = scale};
}

int RenderFrameContext_render_tree(RenderFrameContext* self, WidgetNode* root) {
(self->frame_index = ((*self).frame_index + 0LL));
return 1;
}

