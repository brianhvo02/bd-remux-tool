#pragma once

#include <napi.h>
#include <libbluray/bluray.h>

class Bluray : public Napi::ObjectWrap<Bluray> {
public:
    Bluray(const Napi::CallbackInfo&);
    static Napi::Value Version(const Napi::CallbackInfo&);
    Napi::Value Open(const Napi::CallbackInfo&);
    Napi::Value IsOpen(const Napi::CallbackInfo&);
    Napi::Value Close(const Napi::CallbackInfo&);

    static Napi::Function GetClass(Napi::Env);

private:
    std::string _path;
    BLURAY* _bd;
};
