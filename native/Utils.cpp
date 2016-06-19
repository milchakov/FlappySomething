#include "Utils.h"

#include <memory>


GLuint OGLHelper::CreateProgram(const char* pVertexSource, const char* pFragmentSource) 
{
    GLuint vertexShader = LoadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) 
    {
        return 0;
    }

    GLuint pixelShader = LoadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) 
    {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        LOG_GL_ERRORS("glAttachShader");
        glAttachShader(program, pixelShader);
        LOG_GL_ERRORS("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) 
        {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) 
            {
                std::unique_ptr <char[]> buf(new char[bufLength]);
                if (buf) 
                {
                    glGetProgramInfoLog(program, bufLength, NULL, buf.get());
                    LOG_ERROR("Could not link program: %s;", buf.get());
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

GLuint OGLHelper::LoadShader(GLenum shaderType, const char* pSource) 
{
    GLuint shader = glCreateShader(shaderType);
    LOG_GL_ERRORS("glCreateShader");

    if (shader) 
    {
        glShaderSource(shader, 1, &pSource, NULL);
        LOG_GL_ERRORS("glShaderSource");
        
        glCompileShader(shader);
        LOG_GL_ERRORS("glCompileShader");
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) 
        {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) 
            {
                std::unique_ptr <char[]> buf(new char[infoLen]);
                if (buf) 
                {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf.get());
                    LOG_ERROR("Could not compile shader %d: %s;", shaderType, buf.get());
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint OGLHelper::LoadTexture(Texture &texture)
{
    GLuint textureId;
    glGenTextures(1, &textureId);

    glBindTexture(GL_TEXTURE_2D, textureId);
    LOG_GL_ERRORS("glBindTexture");
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.GetWidth(), texture.GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.GetPixels());
    LOG_GL_ERRORS("glTexImage2D");

    return textureId;
}