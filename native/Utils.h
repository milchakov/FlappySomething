#pragma once

#ifndef utils_h
#define utils_h

#include <android/log.h>
#include <GLES2/gl2.h>

#include <vector>
#include <cstdint>

// uncomment to enable debug logs
//#define DEBUG 

#define  APP_NAME "FlappySomething"

#define  LOG_ERROR(...)  __android_log_print(ANDROID_LOG_ERROR, APP_NAME, __VA_ARGS__)

#ifdef DEBUG    // logging info and GL errors just in debug mode

#define  LOG_INFO(...)  __android_log_print(ANDROID_LOG_INFO, APP_NAME, __VA_ARGS__)
#define LOG_GL_ERRORS(operationName) \
    for (GLint error = glGetError(); error; error = glGetError()) \
{ \
    LOG_ERROR("after %s() glError (0x%x)\n", operationName, error); \
}

#else

#define  LOG_INFO(...)
#define LOG_GL_ERRORS(operationName)

#endif


struct Point
{
    Point()
        : x(0.0f)
        , y(0.0f)
    {}

    Point(GLfloat xPos, GLfloat yPos)
        : x(xPos)
        , y(yPos)
    {}

    GLfloat x;
    GLfloat y;
};


template <typename TPixelType>
class TextureT
{
public:

    TextureT()
        : m_width(0)
        , m_height(0)
    {}

    void SetSize(size_t width, size_t height)
    {
        m_width = width;
        m_height = height;
        m_bitmap.resize(width * height);
    }

    void Clear()
    {
        SetSize(0,0);
    }

    size_t GetWidth()   const
    {
        return m_width;
    }

    size_t GetHeight()   const
    {
        return m_height;
    }

    TPixelType* GetPixels()
    {
        return m_bitmap.data();
    }

    size_t GetCount() const 
    {
        return m_bitmap.size();
    }

private:

    std::vector <TPixelType> m_bitmap;
    size_t m_width;
    size_t m_height;
};

using Texture = TextureT<std::uint32_t>;


/*
Class which contains typical OpenGL operations
*/
class OGLHelper
{
public:

    static GLuint CreateProgram(const char* pVertexSource, const char* pFragmentSource);
    static GLuint LoadTexture(Texture &texture);

private:

    static GLuint LoadShader(GLenum shaderType, const char* pSource);
};


#endif  //utils_h