#include "TextureLoader.h"

#include <cstring>  // memcpy for bitmap data copying
#include <stdexcept>

namespace //anonymous namespace, helpers
{
    
    void SwapColorBytes(std::uint32_t *pixels, size_t size)
    {
        for(size_t i = 0; i < size; ++i)
        {
            pixels[i] = 
                 (pixels[i] & 0xFF000000) |
                ((pixels[i] & 0x00FF0000) >> 16) | 
                 (pixels[i] & 0x0000FF00) | 
                ((pixels[i] & 0x000000FF) << 16);
        }
    }
}   //anonymous namespace

// can not to use std::exceprion::what() with __android_log_print
// due to the compiler error [-Werror,-Wformat-security]
// using this macro instead. 1) log error and 2)throw
#define THROW_ON_ERROR_JNI_CALL(jni_call, result_var, text_on_error) \
    result_var = jni_call; \
    if(!result_var) \
{ \
    LOG_ERROR(text_on_error); \
    throw std::runtime_error(text_on_error); \
} 



TextureLoader::TextureLoader()
    : m_initialized(false)
{
}

TextureLoader::~TextureLoader()
{
    if(!m_initialized)
    {
        return;
    }

    JNIEnv *env;
    if(JNI_OK != m_jvm->AttachCurrentThread(&env, NULL))
    {
        LOG_ERROR("TextureLoader::~TextureLoader() cannot to attach jvm to current thread");
        return;
    }

    env->DeleteGlobalRef(m_javaBitmapLoader);
}

void TextureLoader::Init(JNIEnv* env, jobject bitmapLoader)
{
    if(m_initialized)
    {
        return;
    }

    if(JNI_OK != env->GetJavaVM(&m_jvm))
    {
        return;
    }

    m_javaBitmapLoader = env->NewGlobalRef(bitmapLoader);

    jclass loaderClass = env->GetObjectClass(m_javaBitmapLoader);
    m_loadBitmapId = env->GetMethodID(loaderClass, "LoadBitmap", "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    m_getBitmapWidthId = env->GetMethodID(loaderClass, "GetBitmapWidth", "(Landroid/graphics/Bitmap;)I");
    m_getBitmapHeightId = env->GetMethodID(loaderClass, "GetBitmapHeight", "(Landroid/graphics/Bitmap;)I");
    m_getBitmapDataId = env->GetMethodID(loaderClass, "GetBitmapData", "(Landroid/graphics/Bitmap;[I)V");
    m_releaseBitmapId = env->GetMethodID(loaderClass, "ReleaseBitmap", "(Landroid/graphics/Bitmap;)V");

    m_initialized = true;
}

bool TextureLoader::LoadTexture(Texture &texture, const char *path) const
{
    if(!m_initialized)
    {
        return false;
    }

    JNIEnv *env;
    if(JNI_OK != m_jvm->AttachCurrentThread(&env, NULL))
    {
        LOG_ERROR("TextureLoader::LoadTexture cannot to attach jvm to current thread");
        return false;
    }

    jobject bitmapObject;
    jint width;
    jint height;
    jintArray javaArray;
    jint *bitmapData;

    texture.Clear();

    try
    {
        jstring fileName = env->NewStringUTF(path);

        THROW_ON_ERROR_JNI_CALL(
            env->CallObjectMethod(m_javaBitmapLoader, m_loadBitmapId, fileName),
            bitmapObject,
            "TextureLoader::LoadTexture file is not found");

        THROW_ON_ERROR_JNI_CALL(
            env->CallIntMethod(m_javaBitmapLoader, m_getBitmapWidthId, bitmapObject),
            width,
            "TextureLoader::LoadTexture incorrect bitmap, width = 0");

        THROW_ON_ERROR_JNI_CALL(
            env->CallIntMethod(m_javaBitmapLoader, m_getBitmapHeightId, bitmapObject),
            height,
            "TextureLoader::LoadTexture incorrect bitmap, height = 0");

        THROW_ON_ERROR_JNI_CALL(
            env->NewIntArray(width * height),
            javaArray,
            "TextureLoader::LoadTexture cannot to allocate memory in JVM");

        env->CallVoidMethod(m_javaBitmapLoader, m_getBitmapDataId, bitmapObject, javaArray);

        THROW_ON_ERROR_JNI_CALL(
            env->GetIntArrayElements(javaArray, 0),
            bitmapData,
            "TextureLoader::LoadTexture cannot to GetIntArrayElements");

        try
        {
            texture.SetSize(width, height);
            memcpy(texture.GetPixels(), bitmapData, width * height * sizeof(std::uint32_t));
            SwapColorBytes(texture.GetPixels(), width * height);
        }
        catch(const std::bad_alloc &ex)
        {
            texture.Clear();
        }

        env->ReleaseIntArrayElements(javaArray, bitmapData, 0);
        env->CallVoidMethod(m_javaBitmapLoader, m_releaseBitmapId, bitmapObject);
    }
    catch(const std::runtime_error &re) // can not to use re.what() to log error string, using logging from macro instead
    {
        if(bitmapObject)
        {
            env->CallVoidMethod(m_javaBitmapLoader, m_releaseBitmapId, bitmapObject);
        }
    }

    return (texture.GetCount() > 0);
}