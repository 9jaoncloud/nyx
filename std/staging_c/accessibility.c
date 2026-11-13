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

#define TAG_AccessibleRole_Window 0
#define TAG_AccessibleRole_Button 1
#define TAG_AccessibleRole_Checkbox 2
#define TAG_AccessibleRole_TextInput 3
#define TAG_AccessibleRole_Heading 4
#define TAG_AccessibleRole_Navigation 5
#define TAG_AccessibleRole_Dialog 6
#define TAG_AccessibleRole_Alert 7
typedef struct {
    int tag;
    union {
    } data;
} AccessibleRole;

typedef struct {
    int64_t id;
    AccessibleRole role;
    rt_string_t label_utf8;
    rt_string_t value_utf8;
    int is_focused;
    int is_expanded;
    int is_selected;
    int64_t parent_id;
    int64_t child_count;
    NyxVec child_ids;
} AccessibleNode;

typedef struct {
    int64_t node_count;
    NyxVec nodes;
    int64_t focused_node_id;
} AccessibilityTree;

AccessibleNode accessibility_node_init(int64_t, AccessibleRole, rt_string_t);
// module accessibility
// import prelude::*
AccessibleNode accessibility_node_init(int64_t id, AccessibleRole role, rt_string_t label) {
Arena* __arena = arena_create();
arena_destroy(__arena);
return (AccessibleNode){.id = id, .role = role, .label_utf8 = label, .value_utf8 = rt_string_from(""), .is_focused = 0, .is_expanded = 0, .is_selected = 0, .parent_id = 0LL, .child_count = 0LL, .child_ids = rt_vec_new(sizeof(int64_t))};
}

