#pragma once

#ifndef texture_loader_h
#define texture_loader_h

#include <jni.h>

#include "Utils.h"


/*
Class for texture loading calls java methods by using JNI.
Used BitmapFactory on java side.
Correct working with 32-bits textures only.
*/
class TextureLoader
{
public:

    TextureLoader();
    virtual ~TextureLoader();

    TextureLoader(const TextureLoader &) = delete;          //non-copyable
    TextureLoader &operator=(const TextureLoader &) = delete;

    void Init(JNIEnv* env, jobject bitmapLoader);
    bool LoadTexture(Texture &texture, const char *path) const;

private:

    bool m_initialized;

    JavaVM* m_jvm;
    jobject m_javaBitmapLoader;

    jmethodID m_loadBitmapId;
    jmethodID m_getBitmapWidthId;
    jmethodID m_getBitmapHeightId;
    jmethodID m_getBitmapDataId;
    jmethodID m_releaseBitmapId;


};

#endif  //texture_loader_h