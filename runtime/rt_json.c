#include "rt_json.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

static const char* g_input;
static size_t g_pos;
static size_t g_len;

static void skip_ws(void) {
    while (g_pos < g_len && isspace((unsigned char)g_input[g_pos])) g_pos++;
}

static JsonVal* parse_value(void);

static JsonVal* parse_string_raw(void) {
    g_pos++;
    char* buf = NULL;
    size_t buflen = 0, bufcap = 0;
    while (g_pos < g_len && g_input[g_pos] != '"') {
        char c;
        if (g_input[g_pos] == '\\') {
            g_pos++;
            if (g_pos >= g_len) { free(buf); return NULL; }
            switch (g_input[g_pos]) {
                case '"': c = '"'; break; case '\\': c = '\\'; break;
                case '/': c = '/'; break; case 'n': c = '\n'; break;
                case 't': c = '\t'; break; case 'r': c = '\r'; break;
                case 'b': c = '\b'; break; case 'f': c = '\f'; break;
                case 'u': {
                    g_pos++;
                    unsigned cp = 0;
                    for (int i = 0; i < 4 && g_pos < g_len; i++, g_pos++) {
                        char h = g_input[g_pos]; cp <<= 4;
                        if (h>='0'&&h<='9') cp|=(h-'0');
                        else if (h>='a'&&h<='f') cp|=(h-'a'+10);
                        else if (h>='A'&&h<='F') cp|=(h-'A'+10);
                        else { free(buf); return NULL; }
                    }
                    g_pos--; c = (char)cp; break;
                }
                default: free(buf); return NULL;
            }
        } else { c = g_input[g_pos]; }
        if (buflen+1 >= bufcap) { bufcap = bufcap==0?64:bufcap*2; buf=(char*)realloc(buf,bufcap); }
        buf[buflen++] = c; g_pos++;
    }
    if (g_pos >= g_len) { free(buf); return NULL; }
    g_pos++;
    JsonVal* v = (JsonVal*)malloc(sizeof(JsonVal));
    v->type = JSON_STRING;
    v->data.str_val.data = buf ? buf : strdup("");
    v->data.str_val.length = buflen;
    v->data.str_val.ref_count = 1;
    return v;
}

static JsonVal* parse_number(void) {
    size_t start = g_pos;
    if (g_input[g_pos]=='-') g_pos++;
    while (g_pos<g_len && isdigit((unsigned char)g_input[g_pos])) g_pos++;
    if (g_pos<g_len && g_input[g_pos]=='.') { g_pos++; while (g_pos<g_len && isdigit((unsigned char)g_input[g_pos])) g_pos++; }
    if (g_pos<g_len && (g_input[g_pos]=='e'||g_input[g_pos]=='E')) {
        g_pos++; if (g_pos<g_len && (g_input[g_pos]=='+'||g_input[g_pos]=='-')) g_pos++;
        while (g_pos<g_len && isdigit((unsigned char)g_input[g_pos])) g_pos++;
    }
    char tmp[256]; size_t n = g_pos-start;
    if (n>255) n=255; memcpy(tmp,g_input+start,n); tmp[n]='\0';
    JsonVal* v = (JsonVal*)malloc(sizeof(JsonVal));
    v->type = JSON_NUMBER; v->data.num_val = atof(tmp);
    return v;
}

static JsonVal* parse_array(void) {
    g_pos++;
    JsonVal* v = (JsonVal*)malloc(sizeof(JsonVal));
    v->type=JSON_ARRAY; v->data.arr.items=NULL; v->data.arr.len=0;
    skip_ws();
    if (g_input[g_pos]==']') { g_pos++; return v; }
    size_t cap=0;
    while (1) {
        JsonVal* elem = parse_value();
        if (!elem) { json_free(v); return NULL; }
        if (v->data.arr.len>=cap) { cap=cap==0?8:cap*2; v->data.arr.items=(JsonVal**)realloc(v->data.arr.items,cap*sizeof(JsonVal*)); }
        v->data.arr.items[v->data.arr.len++]=elem;
        skip_ws();
        if (g_input[g_pos]==',') { g_pos++; skip_ws(); continue; }
        if (g_input[g_pos]==']') { g_pos++; return v; }
        json_free(v); return NULL;
    }
}

static JsonVal* parse_object(void) {
    g_pos++;
    JsonVal* v = (JsonVal*)malloc(sizeof(JsonVal));
    v->type=JSON_OBJECT; v->data.obj.keys=NULL; v->data.obj.vals=NULL; v->data.obj.len=0;
    skip_ws();
    if (g_input[g_pos]=='}') { g_pos++; return v; }
    size_t cap=0;
    while (1) {
        JsonVal* key_val = parse_string_raw();
        if (!key_val) { json_free(v); return NULL; }
        rt_string_t key = key_val->data.str_val;
        free(key_val);
        skip_ws(); if (g_input[g_pos]!=':') { json_free(v); return NULL; }
        g_pos++; skip_ws();
        JsonVal* elem = parse_value();
        if (!elem) { json_free(v); return NULL; }
        if (v->data.obj.len>=cap) {
            cap=cap==0?8:cap*2;
            v->data.obj.keys=(rt_string_t*)realloc(v->data.obj.keys,cap*sizeof(rt_string_t));
            v->data.obj.vals=(JsonVal**)realloc(v->data.obj.vals,cap*sizeof(JsonVal*));
        }
        v->data.obj.keys[v->data.obj.len]=key;
        v->data.obj.vals[v->data.obj.len]=elem;
        v->data.obj.len++;
        skip_ws();
        if (g_input[g_pos]==',') { g_pos++; skip_ws(); continue; }
        if (g_input[g_pos]=='}') { g_pos++; return v; }
        json_free(v); return NULL;
    }
}

static JsonVal* parse_value(void) {
    skip_ws();
    if (g_pos>=g_len) return NULL;
    char c = g_input[g_pos];
    if (c=='"') return parse_string_raw();
    if (c=='{') return parse_object();
    if (c=='[') return parse_array();
    if (c=='-'||(c>='0'&&c<='9')) return parse_number();
    JsonVal* v;
    if (g_pos+4<=g_len && strncmp(g_input+g_pos,"true",4)==0) {
        g_pos+=4; v=(JsonVal*)malloc(sizeof(JsonVal)); v->type=JSON_BOOL; v->data.bool_val=1; return v;
    }
    if (g_pos+5<=g_len && strncmp(g_input+g_pos,"false",5)==0) {
        g_pos+=5; v=(JsonVal*)malloc(sizeof(JsonVal)); v->type=JSON_BOOL; v->data.bool_val=0; return v;
    }
    if (g_pos+4<=g_len && strncmp(g_input+g_pos,"null",4)==0) {
        g_pos+=4; v=(JsonVal*)malloc(sizeof(JsonVal)); v->type=JSON_NULL; return v;
    }
    return NULL;
}

// parse(rt_string_t) -> NyxResult
// Result.Ok: data.ptr_val = JsonVal* (as void*)
// Result.Err: data.ptr_val = char* error message
NyxResult parse(rt_string_t input) {
    g_input = input.data;
    g_pos = 0;
    g_len = input.length;
    JsonVal* val = parse_value();
    NyxResult r;
    if (val) {
        skip_ws();
        if (g_pos < g_len) { json_free(val); r.tag=RESULT_ERR; r.data.ptr_val=(void*)strdup("Trailing content"); }
        else { r.tag=RESULT_OK; r.data.ptr_val=(void*)val; }
    } else {
        r.tag=RESULT_ERR; r.data.ptr_val=(void*)strdup("Parse error");
    }
    return r;
}

void json_free(JsonVal* v) {
    if (!v) return;
    switch (v->type) {
        case JSON_STRING: free(v->data.str_val.data); break;
        case JSON_ARRAY: for (size_t i=0;i<v->data.arr.len;i++) json_free(v->data.arr.items[i]); free(v->data.arr.items); break;
        case JSON_OBJECT:
            for (size_t i=0;i<v->data.obj.len;i++) { free(v->data.obj.keys[i].data); json_free(v->data.obj.vals[i]); }
            free(v->data.obj.keys); free(v->data.obj.vals); break;
        default: break;
    }
    free(v);
}

static void write_string(char** buf, size_t* len, size_t* cap, const char* s, size_t slen) {
    if (*len+slen+1 >= *cap) { *cap=(*cap==0?256:(*len+slen)*2); *buf=(char*)realloc(*buf,*cap); }
    memcpy(*buf+*len, s, slen); *len += slen; (*buf)[*len]='\0';
}

static void stringify_rec(JsonVal* v, char** buf, size_t* len, size_t* cap) {
    char tmp[256];
    switch (v->type) {
        case JSON_NULL: write_string(buf,len,cap,"null",4); return;
        case JSON_BOOL: write_string(buf,len,cap,v->data.bool_val?"true":"false",v->data.bool_val?4:5); return;
        case JSON_NUMBER:
            if (v->data.num_val==(int64_t)v->data.num_val) snprintf(tmp,256,"%.0f",v->data.num_val);
            else snprintf(tmp,256,"%g",v->data.num_val);
            write_string(buf,len,cap,tmp,strlen(tmp)); return;
        case JSON_STRING: {
            write_string(buf,len,cap,"\"",1);
            for (size_t i=0;i<v->data.str_val.length;i++) {
                char c=v->data.str_val.data[i];
                if (c=='"'||c=='\\') { write_string(buf,len,cap,"\\",1); }
                write_string(buf,len,cap,&c,1);
            }
            write_string(buf,len,cap,"\"",1); return;
        }
        case JSON_ARRAY:
            write_string(buf,len,cap,"[",1);
            for (size_t i=0;i<v->data.arr.len;i++) {
                if (i>0) write_string(buf,len,cap,",",1);
                stringify_rec(v->data.arr.items[i],buf,len,cap);
            }
            write_string(buf,len,cap,"]",1); return;
        case JSON_OBJECT:
            write_string(buf,len,cap,"{",1);
            for (size_t i=0;i<v->data.obj.len;i++) {
                if (i>0) write_string(buf,len,cap,",",1);
                write_string(buf,len,cap,"\"",1);
                write_string(buf,len,cap,v->data.obj.keys[i].data,v->data.obj.keys[i].length);
                write_string(buf,len,cap,"\"",1);
                write_string(buf,len,cap,":",1);
                stringify_rec(v->data.obj.vals[i],buf,len,cap);
            }
            write_string(buf,len,cap,"}",1); return;
    }
}

// stringify(void* val_ptr) -> rt_string_t
// val_ptr is &JsonVal* (double-indirected because Nyx passes &value)
rt_string_t stringify(void* val_ptr) {
    JsonVal** vp = (JsonVal**)val_ptr;
    JsonVal* v = *vp;
    char* buf=NULL; size_t len=0, cap=0;
    stringify_rec(v, &buf, &len, &cap);
    rt_string_t s;
    if (!buf) { s.data=strdup(""); s.length=0; s.ref_count=1; return s; }
    s.data=buf; s.length=len; s.ref_count=1; return s;
}

rt_string_t pretty(rt_string_t json_val) {
    return json_val;
}
