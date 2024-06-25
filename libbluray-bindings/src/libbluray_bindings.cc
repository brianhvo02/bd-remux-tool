#include "libbluray_bindings.h"

using namespace Napi;

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
    version.Set("major", major);
    version.Set("minor", minor);
    version.Set("micro", micro);
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

Napi::Function Bluray::GetClass(Napi::Env env) {
    return DefineClass(env, "Bluray", {
        Bluray::StaticMethod("version", &Bluray::Version),
        Bluray::InstanceMethod("open", &Bluray::Open),
        Bluray::InstanceMethod("isOpen", &Bluray::IsOpen),
        Bluray::InstanceMethod("close", &Bluray::Close),
    });
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    Napi::String name = Napi::String::New(env, "Bluray");
    exports.Set(name, Bluray::GetClass(env));
    return exports;
}

NODE_API_MODULE(addon, Init)
