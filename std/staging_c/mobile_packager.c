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
#include "runtime/rt_atomic.h"
#include "runtime/nyx_generational.h"
#include "include/nyx_multidialect.h"
#ifndef NYX_CURRENT_CONTEXT_DEFINED
#define NYX_CURRENT_CONTEXT_DEFINED
/* Odin-inspired Native Execution Context */
typedef struct NyxContext {
    void* nursery;
    void* temp_nursery;
    uint64_t thread_id;
    void (*logger)(const char* msg);
    void* user_data;
} NyxContext;
#if defined(_MSC_VER)
__declspec(selectany) __declspec(thread) NyxContext nyx_current_context = {0};
#elif defined(__GNUC__) || defined(__clang__)
__attribute__((weak)) __thread NyxContext nyx_current_context = {0};
#else
static __thread NyxContext nyx_current_context = {0};
#endif
#endif
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

typedef struct {
    rt_string_t app_id;
    rt_string_t app_name;
    rt_string_t version_name;
    int64_t version_code;
    int64_t min_sdk_version;
    int64_t target_sdk_version;
    rt_string_t ios_deployment_target;
    rt_string_t icon_path;
    NyxVec permissions;
} MobilePackageConfig;

rt_string_t generate_android_manifest(MobilePackageConfig*);
rt_string_t generate_ios_info_plist(MobilePackageConfig*);
rt_string_t generate_cmake_native_build(rt_string_t);

#line 1 "mobile_packager.nyx"
// module mobile_packager
// import std::string
// import std::fs
// import std::io
// import std::vec
rt_string_t generate_android_manifest(MobilePackageConfig* config) {
Arena* __arena = arena_create();
rt_string_t xml = int64_t_from(String, rt_string_from("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"));
rt_string_push_str(xml, rt_string_from("<manifest xmlns:android=\"http://schemas.android.com/apk/res/android\"\n"));
rt_string_push_str(xml, rt_string_from("    package=\""));
rt_string_push_str(xml, (&((*config).app_id)));
rt_string_push_str(xml, rt_string_from("\">\n\n"));
rt_string_push_str(xml, rt_string_from("    <uses-feature android:name=\"android.hardware.vulkan.version\" android:version=\"0x403000\" android:required=\"true\" />\n"));
rt_string_push_str(xml, rt_string_from("    <uses-permission android:name=\"android.permission.VIBRATE\" />\n"));
rt_string_push_str(xml, rt_string_from("    <uses-permission android:name=\"android.permission.INTERNET\" />\n"));
rt_string_push_str(xml, rt_string_from("    <uses-permission android:name=\"android.permission.ACCESS_NETWORK_STATE\" />\n\n"));
rt_string_push_str(xml, rt_string_from("    <application\n"));
rt_string_push_str(xml, rt_string_from("        android:allowBackup=\"true\"\n"));
rt_string_push_str(xml, rt_string_from("        android:icon=\"@mipmap/ic_launcher\"\n"));
rt_string_push_str(xml, rt_string_from("        android:label=\""));
rt_string_push_str(xml, (&((*config).app_name)));
rt_string_push_str(xml, rt_string_from("\"\n"));
rt_string_push_str(xml, rt_string_from("        android:theme=\"@style/Theme.NyxApp.Fullscreen\"\n"));
rt_string_push_str(xml, rt_string_from("        android:hasCode=\"true\">\n\n"));
rt_string_push_str(xml, rt_string_from("        <activity\n"));
rt_string_push_str(xml, rt_string_from("            android:name=\".NyxNativeActivity\"\n"));
rt_string_push_str(xml, rt_string_from("            android:exported=\"true\"\n"));
rt_string_push_str(xml, rt_string_from("            android:configChanges=\"orientation|keyboardHidden|screenSize|screenLayout|density\">\n"));
rt_string_push_str(xml, rt_string_from("            <meta-data android:name=\"android.app.lib_name\" android:value=\"nyx_app_native\" />\n"));
rt_string_push_str(xml, rt_string_from("            <intent-filter>\n"));
rt_string_push_str(xml, rt_string_from("                <action android:name=\"android.intent.action.MAIN\" />\n"));
rt_string_push_str(xml, rt_string_from("                <category android:name=\"android.intent.category.LAUNCHER\" />\n"));
rt_string_push_str(xml, rt_string_from("            </intent-filter>\n"));
rt_string_push_str(xml, rt_string_from("        </activity>\n"));
rt_string_push_str(xml, rt_string_from("    </application>\n"));
rt_string_push_str(xml, rt_string_from("</manifest>\n"));
arena_destroy(__arena);
return xml;
}

rt_string_t generate_ios_info_plist(MobilePackageConfig* config) {
Arena* __arena = arena_create();
rt_string_t plist = int64_t_from(String, rt_string_from("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"));
rt_string_push_str(plist, rt_string_from("<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"));
rt_string_push_str(plist, rt_string_from("<plist version=\"1.0\">\n<dict>\n"));
rt_string_push_str(plist, rt_string_from("    <key>CFBundleIdentifier</key>\n    <string>"));
rt_string_push_str(plist, (&((*config).app_id)));
rt_string_push_str(plist, rt_string_from("</string>\n"));
rt_string_push_str(plist, rt_string_from("    <key>CFBundleName</key>\n    <string>"));
rt_string_push_str(plist, (&((*config).app_name)));
rt_string_push_str(plist, rt_string_from("</string>\n"));
rt_string_push_str(plist, rt_string_from("    <key>CFBundleVersion</key>\n    <string>1</string>\n"));
rt_string_push_str(plist, rt_string_from("    <key>CFBundleShortVersionString</key>\n    <string>"));
rt_string_push_str(plist, (&((*config).version_name)));
rt_string_push_str(plist, rt_string_from("</string>\n"));
rt_string_push_str(plist, rt_string_from("    <key>UIRequiredDeviceCapabilities</key>\n    <array>\n        <string>metal</string>\n        <string>arm64</string>\n    </array>\n"));
rt_string_push_str(plist, rt_string_from("    <key>UIViewControllerBasedStatusBarAppearance</key>\n    <false/>\n"));
rt_string_push_str(plist, rt_string_from("</dict>\n</plist>\n"));
arena_destroy(__arena);
return plist;
}

rt_string_t generate_cmake_native_build(rt_string_t app_name) {
Arena* __arena = arena_create();
rt_string_t cmake = int64_t_from(String, rt_string_from("cmake_minimum_required(VERSION 3.22.1)\n"));
rt_string_push_str(cmake, rt_string_from("project(nyx_app_native LANGUAGES C CXX)\n\n"));
rt_string_push_str(cmake, rt_string_from("add_library(nyx_app_native SHARED\n"));
rt_string_push_str(cmake, rt_string_from("    src/main.c\n"));
rt_string_push_str(cmake, rt_string_from("    runtime/rt_ui.c\n"));
rt_string_push_str(cmake, rt_string_from("    runtime/rt_android.c\n"));
rt_string_push_str(cmake, rt_string_from("    runtime/rt_stdlib.c\n"));
rt_string_push_str(cmake, rt_string_from(")\n\n"));
rt_string_push_str(cmake, rt_string_from("find_library(log-lib log)\n"));
rt_string_push_str(cmake, rt_string_from("find_library(android-lib android)\n"));
rt_string_push_str(cmake, rt_string_from("find_library(vulkan-lib vulkan)\n\n"));
rt_string_push_str(cmake, rt_string_from("target_link_libraries(nyx_app_native\n"));
rt_string_push_str(cmake, rt_string_from("    ${log-lib}\n"));
rt_string_push_str(cmake, rt_string_from("    ${android-lib}\n"));
rt_string_push_str(cmake, rt_string_from("    ${vulkan-lib}\n"));
rt_string_push_str(cmake, rt_string_from(")\n"));
arena_destroy(__arena);
return cmake;
}

