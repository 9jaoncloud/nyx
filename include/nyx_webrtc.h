#ifndef NYX_WEBRTC_H
#define NYX_WEBRTC_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

/* --------------------------------------------------------------------------
 * Nyx Native Zero-GC WebRTC & LiveKit Protocol Subsystem
 * High-performance real-time audio/video signaling & DataChannel engine
 * -------------------------------------------------------------------------- */

typedef enum {
    NYX_WEBRTC_DISCONNECTED = 0,
    NYX_WEBRTC_CONNECTING   = 1,
    NYX_WEBRTC_CONNECTED    = 2,
    NYX_WEBRTC_FAILED       = 3,
    NYX_WEBRTC_CLOSED       = 4
} NyxWebRTCState;

typedef enum {
    NYX_TRACK_AUDIO = 0,
    NYX_TRACK_VIDEO = 1,
    NYX_TRACK_DATA  = 2
} NyxTrackType;

typedef struct {
    char track_id[64];
    NyxTrackType type;
    bool is_muted;
    uint32_t sample_rate;
    uint32_t channels;
    uint32_t bitrate_bps;
} NyxMediaTrack;

typedef struct {
    char room_name[128];
    char participant_identity[128];
    char sfu_url[256];
    NyxWebRTCState state;
    uint32_t active_tracks_count;
    NyxMediaTrack tracks[8];
} NyxLiveKitSession;

/* Session Management */
NyxLiveKitSession* nyx_webrtc_create_session(const char* sfu_url, const char* room_name, const char* participant_identity);
bool nyx_webrtc_connect(NyxLiveKitSession* session, const char* token);
void nyx_webrtc_disconnect(NyxLiveKitSession* session);

/* Media Tracks & Data Channel */
bool nyx_webrtc_publish_audio_track(NyxLiveKitSession* session, const char* track_id, uint32_t sample_rate, uint32_t channels);
bool nyx_webrtc_publish_video_track(NyxLiveKitSession* session, const char* track_id, uint32_t width, uint32_t height, uint32_t fps);
bool nyx_webrtc_send_data_message(NyxLiveKitSession* session, const uint8_t* payload, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* NYX_WEBRTC_H */
