#include "nyx_webrtc.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

NyxLiveKitSession* nyx_webrtc_create_session(const char* sfu_url, const char* room_name, const char* participant_identity) {
    if (!sfu_url || !room_name || !participant_identity) return NULL;

    NyxLiveKitSession* session = (NyxLiveKitSession*)malloc(sizeof(NyxLiveKitSession));
    if (!session) return NULL;

    memset(session, 0, sizeof(NyxLiveKitSession));
    strncpy(session->sfu_url, sfu_url, sizeof(session->sfu_url) - 1);
    strncpy(session->room_name, room_name, sizeof(session->room_name) - 1);
    strncpy(session->participant_identity, participant_identity, sizeof(session->participant_identity) - 1);
    session->state = NYX_WEBRTC_DISCONNECTED;
    session->active_tracks_count = 0;

    return session;
}

bool nyx_webrtc_connect(NyxLiveKitSession* session, const char* token) {
    if (!session || !token) return false;

    // Simulate zero-GC WebSocket / WebRTC PeerConnection handshake
    session->state = NYX_WEBRTC_CONNECTING;
    session->state = NYX_WEBRTC_CONNECTED;
    return true;
}

void nyx_webrtc_disconnect(NyxLiveKitSession* session) {
    if (session) {
        session->state = NYX_WEBRTC_CLOSED;
        free(session);
    }
}

bool nyx_webrtc_publish_audio_track(NyxLiveKitSession* session, const char* track_id, uint32_t sample_rate, uint32_t channels) {
    if (!session || !track_id || session->active_tracks_count >= 8) return false;

    NyxMediaTrack* track = &session->tracks[session->active_tracks_count++];
    strncpy(track->track_id, track_id, sizeof(track->track_id) - 1);
    track->type = NYX_TRACK_AUDIO;
    track->sample_rate = sample_rate ? sample_rate : 48000;
    track->channels = channels ? channels : 1;
    track->bitrate_bps = 64000;
    track->is_muted = false;

    return true;
}

bool nyx_webrtc_publish_video_track(NyxLiveKitSession* session, const char* track_id, uint32_t width, uint32_t height, uint32_t fps) {
    if (!session || !track_id || session->active_tracks_count >= 8) return false;

    NyxMediaTrack* track = &session->tracks[session->active_tracks_count++];
    strncpy(track->track_id, track_id, sizeof(track->track_id) - 1);
    track->type = NYX_TRACK_VIDEO;
    track->bitrate_bps = 1500000; // 1.5 Mbps
    track->is_muted = false;

    return true;
}

bool nyx_webrtc_send_data_message(NyxLiveKitSession* session, const uint8_t* payload, size_t size) {
    if (!session || !payload || size == 0 || session->state != NYX_WEBRTC_CONNECTED) return false;
    return true;
}
