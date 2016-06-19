#include <stdlib.h>
#include <jni.h>
#include "..\native\FlappySomething.h"


extern "C"
{
    JNIEXPORT void JNICALL Java_app_FlappySomething_FlappySomethingJavaToNative_OnCreate(JNIEnv* env, jobject obj, jobject bitmapLoader)
    {
        FlappySomethingEngine::Instance().InitTextureLoader(env, bitmapLoader);
    }
    
    JNIEXPORT void JNICALL Java_app_FlappySomething_FlappySomethingJavaToNative_OnStop(JNIEnv* env, jobject obj)
    {
         FlappySomethingEngine::Instance().OnStop();
    }
    
    JNIEXPORT void JNICALL Java_app_FlappySomething_FlappySomethingJavaToNative_SetSurfaceSize(JNIEnv* env, jclass clazz, int Width, int Height)
    {
        FlappySomethingEngine::Instance().OnInit(Width, Height);
    }

    JNIEXPORT void JNICALL Java_app_FlappySomething_FlappySomethingJavaToNative_OnDraw(JNIEnv* env, jobject obj)
    {
        FlappySomethingEngine::Instance().OnDraw();
    } 

    JNIEXPORT void JNICALL Java_app_FlappySomething_FlappySomethingJavaToNative_OnTouchDown(JNIEnv* env, jobject obj, float xPos, float yPos)
    {
        FlappySomethingEngine::Instance().OnTouch(xPos, yPos);
    }

} // extern "C"