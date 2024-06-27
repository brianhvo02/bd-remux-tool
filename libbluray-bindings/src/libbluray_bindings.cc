#include "libbluray_bindings.h"

using namespace Napi;

static std::string _get_string(const char* str) {
    return str == NULL ? "" : std::string(str);
}

static Napi::Object _get_bluray_title(Napi::Env env, const BLURAY_TITLE* title) {
    Napi::Object title_obj = Napi::Object::New(env);

    title_obj["name"] = _get_string(title->name);
    title_obj["interactive"] = Napi::Boolean::New(env, title->interactive);
    title_obj["accessible"] = Napi::Boolean::New(env, title->accessible);
    title_obj["hidden"] = Napi::Boolean::New(env, title->hidden);
    title_obj["bdj"] = Napi::Boolean::New(env, title->bdj);
    title_obj["idRef"] = title->id_ref;

    return title_obj;
}

static Napi::Object _get_bluray_stream_info(Napi::Env env, const BLURAY_STREAM_INFO* stream_info) {
    Napi::Object stream_info_obj = Napi::Object::New(env);

    stream_info_obj["codingType"] = stream_info->coding_type;
    stream_info_obj["format"] = stream_info->format;
    stream_info_obj["rate"] = stream_info->rate;
    stream_info_obj["charCode"] = stream_info->char_code;

    Napi::Array lang = Napi::Array::New(env, sizeof(stream_info->lang));
    for (uint8_t i = 0; i < sizeof(stream_info->lang); i++) {
        lang[i] = stream_info->lang[i];
    }
    stream_info_obj["lang"] = lang;
    
    stream_info_obj["pid"] = stream_info->pid;
    stream_info_obj["aspect"] = stream_info->aspect;
    stream_info_obj["subpathId"] = stream_info->subpath_id;

    return stream_info_obj;
}

static Napi::Object _get_bluray_clip_info(Napi::Env env, const BLURAY_CLIP_INFO* clip_info) {
    Napi::Object clip_info_obj = Napi::Object::New(env);

    clip_info_obj["pktCount"] = clip_info->pkt_count;
    clip_info_obj["stillMode"] = clip_info->still_mode;
    clip_info_obj["stillTime"] = clip_info->still_time;
    clip_info_obj["videoStreamCount"] = clip_info->video_stream_count;
    clip_info_obj["audioStreamCount"] = clip_info->audio_stream_count;
    clip_info_obj["pgStreamCount"] = clip_info->pg_stream_count;
    clip_info_obj["igStreamCount"] = clip_info->ig_stream_count;
    clip_info_obj["secAudioStreamCount"] = clip_info->sec_audio_stream_count;
    clip_info_obj["secVideoStreamCount"] = clip_info->sec_video_stream_count;

    Napi::Array video_streams = Napi::Array::New(env, clip_info->video_stream_count);
    for (uint8_t i = 0; i < clip_info->video_stream_count; i++) {
        video_streams[i] = _get_bluray_stream_info(env, &clip_info->video_streams[i]);
    }
    clip_info_obj["videoStreams"] = video_streams;

    Napi::Array audio_streams = Napi::Array::New(env, clip_info->audio_stream_count);
    for (uint8_t i = 0; i < clip_info->audio_stream_count; i++) {
        audio_streams[i] = _get_bluray_stream_info(env, &clip_info->audio_streams[i]);
    }
    clip_info_obj["audioStreams"] = audio_streams;

    Napi::Array pg_streams = Napi::Array::New(env, clip_info->pg_stream_count);
    for (uint8_t i = 0; i < clip_info->pg_stream_count; i++) {
        pg_streams[i] = _get_bluray_stream_info(env, &clip_info->pg_streams[i]);
    }
    clip_info_obj["pgStreams"] = pg_streams;

    Napi::Array ig_streams = Napi::Array::New(env, clip_info->ig_stream_count);
    for (uint8_t i = 0; i < clip_info->ig_stream_count; i++) {
        ig_streams[i] = _get_bluray_stream_info(env, &clip_info->ig_streams[i]);
    }
    clip_info_obj["igStreams"] = ig_streams;

    Napi::Array sec_audio_streams = Napi::Array::New(env, clip_info->sec_audio_stream_count);
    for (uint8_t i = 0; i < clip_info->sec_audio_stream_count; i++) {
        sec_audio_streams[i] = _get_bluray_stream_info(env, &clip_info->sec_audio_streams[i]);
    }
    clip_info_obj["secAudioStreams"] = sec_audio_streams;

    Napi::Array sec_video_streams = Napi::Array::New(env, clip_info->sec_video_stream_count);
    for (uint8_t i = 0; i < clip_info->sec_video_stream_count; i++) {
        sec_video_streams[i] = _get_bluray_stream_info(env, &clip_info->sec_video_streams[i]);
    }
    clip_info_obj["secVideoStreams"] = sec_video_streams;

    clip_info_obj["startTime"] = clip_info->start_time;
    clip_info_obj["inTime"] = clip_info->in_time;
    clip_info_obj["outTime"] = clip_info->out_time;
    clip_info_obj["clipId"] = _get_string(clip_info->clip_id);

    return clip_info_obj;
}

static Napi::Object _get_bluray_title_chapter(Napi::Env env, const BLURAY_TITLE_CHAPTER* chapter) {
    Napi::Object chapter_obj = Napi::Object::New(env);

    chapter_obj["idx"] = chapter->idx;
    chapter_obj["start"] = chapter->start;
    chapter_obj["duration"] = chapter->duration;
    chapter_obj["offset"] = chapter->offset;
    chapter_obj["clipRef"] = chapter->clip_ref;

    return chapter_obj;
}

static Napi::Object _get_bluray_title_mark(Napi::Env env, const BLURAY_TITLE_MARK* mark) {
    Napi::Object mark_obj = Napi::Object::New(env);

    mark_obj["idx"] = mark->idx;
    mark_obj["type"] = mark->type;
    mark_obj["start"] = mark->start;
    mark_obj["duration"] = mark->duration;
    mark_obj["offset"] = mark->offset;
    mark_obj["clipRef"] = mark->clip_ref;

    return mark_obj;
}

static Napi::Object _get_bluray_title_info(Napi::Env env, const BLURAY_TITLE_INFO* title_info) {
    Napi::Object title_info_obj = Napi::Object::New(env);

    title_info_obj["idx"] = title_info->idx;
    title_info_obj["playlist"] = title_info->playlist;
    title_info_obj["duration"] = title_info->duration;
    title_info_obj["clipCount"] = title_info->clip_count;
    title_info_obj["angleCount"] = title_info->angle_count;
    title_info_obj["chapterCount"] = title_info->chapter_count;
    title_info_obj["markCount"] = title_info->mark_count;

    Napi::Array clips = Napi::Array::New(env, title_info->clip_count);
    for (uint8_t i = 0; i < title_info->clip_count; i++) {
        clips[i] = _get_bluray_clip_info(env, &title_info->clips[i]);
    }
    title_info_obj["clips"] = clips;

    Napi::Array chapters = Napi::Array::New(env, title_info->chapter_count);
    for (uint8_t i = 0; i < title_info->chapter_count; i++) {
        chapters[i] = _get_bluray_title_chapter(env, &title_info->chapters[i]);
    }
    title_info_obj["chapters"] = chapters;

    Napi::Array marks = Napi::Array::New(env, title_info->mark_count);
    for (uint8_t i = 0; i < title_info->mark_count; i++) {
        marks[i] = _get_bluray_title_mark(env, &title_info->marks[i]);
    }
    title_info_obj["marks"] = marks;

    title_info_obj["mvcBaseViewRFlag"] = title_info->mvc_base_view_r_flag;

    return title_info_obj;
}

Bluray::Bluray(const Napi::CallbackInfo& info) : ObjectWrap(info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        this->_bd = bd_init();
    } else {
        if (!info[0].IsString()) {
            Napi::TypeError::New(env, "Path is not a string")
            .ThrowAsJavaScriptException();
            return;
        }

        this->_path = info[0].As<Napi::String>().Utf8Value();
        this->_bd = bd_open(this->_path.c_str(), NULL);
    }

    if (this->_bd == NULL) {
        this->_path = "";
        Napi::TypeError::New(env, "Could not initialize bluray object")
          .ThrowAsJavaScriptException();
        return;
    }
}

Napi::Value Bluray::Version(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    int major;
    int minor;
    int micro;
    bd_get_version(&major, &minor, &micro);
    Napi::Object version = Napi::Object::New(env);
    version["major"] = major;
    version["minor"] = minor;
    version["micro"] = micro;
    return version;
}

Napi::Value Bluray::Open(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "Path required")
          .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, 0);
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Path is not a string")
          .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, 0);
    }

    if (this->_path.length() > 0) {
        Napi::TypeError::New(env, "Bluray disc already opened")
          .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, 0);
    }

    this->_path = info[0].As<Napi::String>().Utf8Value();
    this->_bd = bd_open(this->_path.c_str(), NULL);

    if (this->_bd == NULL) {
        this->_path = "";
        return Napi::Boolean::New(env, 0);
    }

    return Napi::Boolean::New(env, 1);
}

Napi::Value Bluray::IsOpen(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    return Napi::Boolean::New(env, this->_bd != NULL && this->_path.length() > 0);
}

Napi::Value Bluray::Close(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (this->_path.length() == 0) {
        Napi::TypeError::New(env, "Bluray disc not opened")
          .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, 0);
    }

    if (this->_bd == NULL) {
        Napi::TypeError::New(env, "Bluray object not initialized")
          .ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, 0);
    }

    bd_close(this->_bd);
    
    return Napi::Boolean::New(env, this->_bd != NULL && this->_path.length() > 0);
}

Napi::Value Bluray::GetDiscInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (this->_bd == NULL) {
        Napi::TypeError::New(env, "Bluray object not initialized")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    const BLURAY_DISC_INFO* disc_info = bd_get_disc_info(this->_bd);

    if (disc_info == NULL) {
        Napi::TypeError::New(env, "Error reading Bluray disc info")
          .ThrowAsJavaScriptException();
        return env.Null();
    }
    
    Napi::Object info_obj = Napi::Object::New(env);

    info_obj["blurayDetected"] = Napi::Boolean::New(env, disc_info->bluray_detected);
    info_obj["discName"] = _get_string(disc_info->disc_name);
    info_obj["udfVolumeId"] = _get_string(disc_info->udf_volume_id);
    info_obj["discId"] = Napi::ArrayBuffer::New(env, (void *)disc_info->disc_id, sizeof(disc_info->disc_id));
    info_obj["noMenuSupport"] = Napi::Boolean::New(env, disc_info->no_menu_support);
    info_obj["firstPlaySupported"] = Napi::Boolean::New(env, disc_info->first_play_supported);
    info_obj["topMenuSupported"] = Napi::Boolean::New(env, disc_info->top_menu_supported);
    info_obj["numTitles"] = disc_info->num_titles;

    Napi::Array titles = Napi::Array::New(env, disc_info->num_titles);
    for (uint32_t i = 1; i <= disc_info->num_titles; i++) {
        titles[i - 1] = _get_bluray_title(env, disc_info->titles[i]);
    }
    info_obj["titles"] = titles;
    info_obj["firstPlay"] = _get_bluray_title(env, disc_info->first_play);
    info_obj["topMenu"] = _get_bluray_title(env, disc_info->top_menu);

    info_obj["numHdmvTitles"] = disc_info->num_hdmv_titles;
    info_obj["numBdjTitles"] = disc_info->num_bdj_titles;
    info_obj["numUnsupportedTitles"] = disc_info->num_unsupported_titles;
    info_obj["bdjDetected"] = Napi::Boolean::New(env, disc_info->bdj_detected);
    info_obj["bdjSupported"] = Napi::Boolean::New(env, disc_info->bdj_supported);
    info_obj["libjvmDetected"] = Napi::Boolean::New(env, disc_info->libjvm_detected);
    info_obj["bdjHandled"] = Napi::Boolean::New(env, disc_info->bdj_handled);
    info_obj["bdjOrgId"] = _get_string(disc_info->bdj_org_id);
    info_obj["bdjDiscId"] = _get_string(disc_info->bdj_disc_id);
    info_obj["videoFormat"] = disc_info->video_format;
    info_obj["frameRate"] = disc_info->frame_rate;
    info_obj["contentExist3D"] = Napi::Boolean::New(env, disc_info->content_exist_3D);
    info_obj["initialOutputModePreference"] = disc_info->initial_output_mode_preference;
    info_obj["providerData"] = Napi::ArrayBuffer::New(env, (void *)disc_info->provider_data, sizeof(disc_info->provider_data));
    info_obj["aacsDetected"] = Napi::Boolean::New(env, disc_info->aacs_detected);
    info_obj["libaacsDetected"] = Napi::Boolean::New(env, disc_info->libaacs_detected);
    info_obj["aacsHandled"] = Napi::Boolean::New(env, disc_info->aacs_handled);
    info_obj["aacsErrorCode"] = disc_info->aacs_error_code;
    info_obj["aacsMkbv"] = disc_info->aacs_mkbv;
    info_obj["bdplusDetected"] = Napi::Boolean::New(env, disc_info->bdplus_detected);
    info_obj["libbdplusDetected"] = Napi::Boolean::New(env, disc_info->libbdplus_detected);
    info_obj["bdplusHandled"] = Napi::Boolean::New(env, disc_info->bdplus_handled);
    info_obj["bdplusGen"] = disc_info->bdplus_gen;
    info_obj["bdplusDate"] = disc_info->bdplus_date;
    info_obj["initialDynamicRangeType"] = disc_info->initial_dynamic_range_type;

    return info_obj;
}

Napi::Value Bluray::GetTitles(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (this->_bd == NULL) {
        Napi::TypeError::New(env, "Bluray object not initialized")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsUndefined() && !info[0].IsNumber()) {
        Napi::TypeError::New(env, "Flags are not numeric")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[1].IsUndefined() && !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Min length is not numeric")
          .ThrowAsJavaScriptException();
        return env.Null();
    }
    
    uint8_t flags = info[0].IsNumber() ? info[0].As<Napi::Number>().Uint32Value() : 0;
    uint32_t min_length = info[1].IsNumber() ? info[1].As<Napi::Number>().Uint32Value() : 0;

    uint32_t title_count = bd_get_titles(this->_bd, flags, min_length);
    return Napi::Number::New(env, title_count);
}

Napi::Value Bluray::GetMainTitle(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();
    
    this->GetTitles(info);

    uint32_t title = bd_get_main_title(this->_bd);
    return Napi::Number::New(env, title);
}

Napi::Value Bluray::GetTitleInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Title index is not numeric")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[1].IsUndefined() && !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Angle is not numeric")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    this->GetTitles(info);

    uint32_t title = info[0].As<Napi::Number>().Uint32Value();
    unsigned angle = info[1].IsNumber() ? info[1].As<Napi::Number>().Uint32Value() : 0;
    const BLURAY_TITLE_INFO* title_info = bd_get_title_info(this->_bd, title, angle);

    if (title_info == NULL) {
        Napi::TypeError::New(env, "Error reading Bluray title info")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    return _get_bluray_title_info(env, title_info);
}

Napi::Value Bluray::GetPlaylistInfo(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    if (this->_bd == NULL) {
        Napi::TypeError::New(env, "Bluray object not initialized")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[0].IsNumber()) {
        Napi::TypeError::New(env, "Playlist id is not numeric")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    if (!info[1].IsUndefined() && !info[1].IsNumber()) {
        Napi::TypeError::New(env, "Angle is not numeric")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    uint32_t playlist_id = info[0].As<Napi::Number>().Uint32Value();
    unsigned angle = info[1].IsNumber() ? info[1].As<Napi::Number>().Uint32Value() : 0;
    const BLURAY_TITLE_INFO* playlist_info = bd_get_playlist_info(this->_bd, playlist_id, angle);

    if (playlist_info == NULL) {
        Napi::TypeError::New(env, "Error reading Bluray title info")
          .ThrowAsJavaScriptException();
        return env.Null();
    }

    return _get_bluray_title_info(env, playlist_info);
}

Napi::Function Bluray::GetClass(Napi::Env env) {
    return DefineClass(env, "Bluray", {
        Bluray::StaticMethod("version", &Bluray::Version),
        Bluray::InstanceMethod("open", &Bluray::Open),
        Bluray::InstanceMethod("isOpen", &Bluray::IsOpen),
        Bluray::InstanceMethod("close", &Bluray::Close),
        Bluray::InstanceMethod("getDiscInfo", &Bluray::GetDiscInfo),
        Bluray::InstanceMethod("getTitles", &Bluray::GetTitles),
        Bluray::InstanceMethod("getMainTitle", &Bluray::GetMainTitle),
        Bluray::InstanceMethod("getTitleInfo", &Bluray::GetTitleInfo),
        Bluray::InstanceMethod("getPlaylistInfo", &Bluray::GetPlaylistInfo)
    });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::String name = Napi::String::New(env, "Bluray");
    exports[name] = Bluray::GetClass(env);
    return exports;
}

NODE_API_MODULE(addon, Init)
