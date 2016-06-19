#include "Menu.h"
#include "FlappySomething.h"


namespace   //anonymous namespace, helpers
{
    const char vertexShader[] =
        "precision lowp float;\n"
        "attribute vec2 positionVec;\n"
        "attribute vec2 textureCoordinateVec;\n"
        "varying vec2 textureCoordinateVary;\n"
        "   \n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(positionVec, 0.0, 1.0);\n"
        "    textureCoordinateVary = textureCoordinateVec;\n"
        "}\n";
    
    const char fragmentShader[] =
        "precision lowp float;\n"
        "varying vec2 textureCoordinateVary;\n"
        "uniform sampler2D textureData;\n"
        "   \n"
        "void main()\n"
        "{\n"
        "    gl_FragColor = texture2D(textureData, textureCoordinateVary);\n"
        "}\n";

    const Point textureVertices[] = 
    {
        Point(1.0f, 0.0f),
        Point(0.0f,  0.0f),

        Point(1.0f, 1.0f),
        Point(0.0f,  1.0f),
    };

}   //anonymous namespace



TexturedMenu::TexturedMenu(const char* imageName)
    : m_imageName(imageName)
    , m_program(0)
{
}

TexturedMenu::~TexturedMenu()
{
}

void TexturedMenu::OnInit(int width, int height)
{
    LOG_INFO("TexturedMenu OnInit method");

    Texture texture;
    if(!FlappySomethingEngine::Instance().LoadTexture(texture, m_imageName.c_str()))
    {
        return;
    }

    m_textureId = OGLHelper::LoadTexture(texture);

    m_program = OGLHelper::CreateProgram(vertexShader, fragmentShader);
    if (!m_program) 
    {
        LOG_ERROR("Could not create program for the TexturedMenu class.");
        return;
    }
    m_positionHandle = glGetAttribLocation(m_program, "positionVec");
    LOG_GL_ERRORS("glGetAttribLocation");
    LOG_INFO("glGetAttribLocation(\"positionVec\") = %d\n", m_positionHandle);

    m_textureCoordsHandle = glGetAttribLocation(m_program, "textureCoordinateVec");
    LOG_GL_ERRORS("textureCoordinateVec");
    LOG_INFO("glGetAttribLocation(\"textureCoordinateVec\") = %d\n", m_textureCoordsHandle);

    m_textureDataHandle = glGetUniformLocation(m_program, "textureData");
    LOG_GL_ERRORS("glGetUniformLocation");
    LOG_INFO("glGetUniformLocation(\"textureData\") = %d\n", m_textureDataHandle);   

    InitPositionArray(texture.GetWidth(), texture.GetHeight(), width, height); 
}


// init position array with scale
void TexturedMenu::InitPositionArray(int texWidth, int texheight, int dispWidth, int dispHeight)
{
    int textureMaxSize = std::max(texWidth, texheight);
    int displayMinSize = std::min(dispWidth, dispHeight);

    GLfloat scale = 1.0f;
    if(textureMaxSize * 2 < displayMinSize)
    {
        scale = 2.0f;
    }

    GLfloat scaledWidth = 2.0f * ((GLfloat)texWidth / (GLfloat)dispWidth) * scale;
    GLfloat scaledHeight = 2.0f * ((GLfloat)texheight / (GLfloat)dispHeight) * scale;

    m_positionArray =   
    {
        Point(scaledWidth / 2.0f, scaledHeight / 2.0f),
        Point(-scaledWidth / 2.0f, scaledHeight / 2.0f),

        Point(scaledWidth / 2.0f, -scaledHeight / 2.0f),
        Point(-scaledWidth / 2.0f, -scaledHeight / 2.0f),
    };
}

void TexturedMenu::OnDraw()
{
    LOG_INFO("TexturedMenu OnDraw method");

    if(!m_program)
    {
        LOG_ERROR("OpenGl program is not initialized for TexturedMenu class");
        return;
    }

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);
    LOG_GL_ERRORS("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    LOG_GL_ERRORS("glClear");

    glUseProgram(m_program);
    LOG_GL_ERRORS("glUseProgram");

    glActiveTexture(GL_TEXTURE0);  
    glBindTexture(GL_TEXTURE_2D, m_textureId);
    
    glUniform1i(m_textureDataHandle, 0);

    glVertexAttribPointer(m_positionHandle, 2, GL_FLOAT, 0, 0, m_positionArray.data());
    glEnableVertexAttribArray(m_positionHandle);
    glVertexAttribPointer(m_textureCoordsHandle, 2, GL_FLOAT, 0, 0, textureVertices);
    glEnableVertexAttribArray(m_textureCoordsHandle);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4); 
    LOG_GL_ERRORS("glDrawArrays");
}

void TexturedMenu::OnTouch(float xPos, float yPos)
{
    FlappySomethingEngine::Instance().SwitchToNextState();
}