#include "Pipes.h"

#include <cstdlib>
#include <ctime>


namespace   //anonymous namespace, helpers
{
    const char vertexShader[] =
        "precision mediump float;\n"
        "attribute vec2 positionVec;\n"
        "uniform float offsetX;\n"
        "uniform float offsetY;\n"
        "   \n"
        "void main() {\n"
        "  gl_Position = vec4(positionVec.x + offsetX, positionVec.y + offsetY, 0.0, 1.0);\n"
        "}\n";

    const char fragmentShader[] = 
        "precision mediump float;\n"
        "   \n"
        "void main() {\n"
        "  gl_FragColor = vec4(124./255., 210./255., 53./255., 1.0f);\n"
        "}\n";


    const Pipes::PipesArray triangleInitialCoords = 
    {
        Point(1.0f, 3.0f), Point(1.0f, 0.2f), Point(1.3f, 0.2f),     // top rect
        Point(1.0f, 3.0f), Point(1.3f, 3.0f), Point(1.3f, 0.2f),

        Point(1.3f, -0.2f), Point(1.0f, -0.2f), Point(1.0f, -3.0f),  // bottom rect
        Point(1.3f, -0.2f), Point(1.3f, -3.0f), Point(1.0f, -3.0f)
    };

    GLfloat GetRandomYOffset()
    {
        return ((GLfloat)(rand() % 80) / 100.0f) - (40.0f / 100.0f);    // generate Y offset value in [-0.4;0.4]
    }

}   //anonymous namespace, helpers



Pipes::Pipes(GLfloat speed)
    : m_triangelsCoords(triangleInitialCoords)         
    , m_speed(speed)
    , m_program(0)
{
    srand(time(nullptr));   // init pseudo-random generator
}

Pipes::~Pipes()
{
}

void Pipes::GenerateOffsets()
{
    for(size_t i = 0; i < m_offsets.size(); ++i)
    {
        m_offsets[i].x = i * NEXT_FIGURE_POS;
        m_offsets[i].y = GetRandomYOffset();
    }
}

void Pipes::OnInit(int width, int height) 
{
    LOG_INFO("Pipes initialization");

    m_program = OGLHelper::CreateProgram(vertexShader, fragmentShader);
    if (!m_program) 
    {
        LOG_ERROR("Could not create program for the Pipes class.");
        return;
    }
    m_positionHandle = glGetAttribLocation(m_program, "positionVec");
    LOG_GL_ERRORS("glGetAttribLocation");
    LOG_INFO("glGetAttribLocation(\"positionVec\") = %d\n", m_positionHandle);

    m_offsetXHandle = glGetUniformLocation(m_program, "offsetX");
    LOG_GL_ERRORS("glGetUniformLocation");
    LOG_INFO("glGetUniformLocation(\"offsetX\") = %d\n", m_offsetXHandle);

    m_offsetYHandle = glGetUniformLocation(m_program, "offsetY");
    LOG_GL_ERRORS("glGetUniformLocation");
    LOG_INFO("glGetUniformLocation(\"offsetY\") = %d\n", m_offsetYHandle);

    GenerateOffsets();
}

void Pipes::OnDraw() 
{
    LOG_INFO("Pipes OnDraw method");

    if(!m_program)
    {
        LOG_ERROR("OpenGl program is not initialized for Pipes class");
        return;
    }

    glUseProgram(m_program);
    LOG_GL_ERRORS("glUseProgram");

    glVertexAttribPointer(m_positionHandle, VERTEX_SIZE, GL_FLOAT, GL_FALSE, 0, m_triangelsCoords.data());
    LOG_GL_ERRORS("glVertexAttribPointer");
    glEnableVertexAttribArray(m_positionHandle);
    LOG_GL_ERRORS("glEnableVertexAttribArray");

    for(int i = 0; i < FIGURES_COUNT; ++i)  // draw one figure three times with different offset
    {
        glUniform1f(m_offsetXHandle, m_offsets[i].x);
        glUniform1f(m_offsetYHandle, m_offsets[i].y);
        glDrawArrays(GL_TRIANGLES, 0, m_triangelsCoords.size());
        LOG_GL_ERRORS("glDrawArrays");
    }
}

void Pipes::OnTouch(float xPos, float yPos) 
{
}

void Pipes::OnStop() 
{
    m_triangelsCoords = triangleInitialCoords;  // revert to base positions on stop

    GenerateOffsets();
}

void Pipes::Iterate(double timeElapsed) 
{
    LOG_INFO("Pipes Iterate method, timeElapsed = %f", timeElapsed);

    GLfloat moveDistance = m_speed * timeElapsed;

    for(int i = 0; i < FIGURES_COUNT; ++i)
    {
        if(!i || (i && (m_offsets[i - 1].x > m_offsets[i].x)))  // using it for float precision correction
        {
            if((m_offsets[i].x - moveDistance) < (-2.0f - FIGURE_WIDTH))    // if figure goes out of screen
            {
                m_offsets[i].x = i * NEXT_FIGURE_POS;
                m_offsets[i].y = GetRandomYOffset();
            }
            else
            {
                m_offsets[i].x -= moveDistance;                 // decrement offset value only for first figure
            }
        }
        else
        {                                                                      
            m_offsets[i].x = m_offsets[i - 1].x + NEXT_FIGURE_POS;  // for other figures use strict offset from previous figure   
        }
    }
}


const Pipes::PipesArray &Pipes::GetTriangleCoords() const
{
    return m_triangelsCoords;
}

const Pipes::OffsetsArray &Pipes::GetOffsets() const
{
    return m_offsets;
}