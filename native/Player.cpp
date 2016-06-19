#include "Player.h"

#include <cmath>

namespace  //anonymous namespace, helpers
 {
    const char vertexShader[] =
        "precision mediump float;\n"
        "attribute vec2 positionVec;\n"
        "uniform float pointSize;\n"
        "   \n"
        "void main() {\n"
        "   gl_PointSize = pointSize;\n"
        "   gl_Position = vec4(positionVec, 0.0, 1.0);\n"
        "}\n";

    const char fragmentShader[] = 
        "precision mediump float;\n"
        "   \n"
        "void main() {\n"
        "   float opacity = 1.0; \n"
        "   vec2 coord = gl_PointCoord - vec2(0.5);\n"
        "   if(length(coord) > 0.5) {\n"
        "       opacity = 0.0; \n"
        "   } \n"
        "     \n"
        "   gl_FragColor = vec4(0.6, 0.0, 0.0, opacity);\n"
        "}\n";

    const Point positionInitialValues(-0.3f, 0.0f);    

}   //anonymous namespace, helpers


Player::Player()
    : m_position(positionInitialValues)
    , m_previousPosition(positionInitialValues)
    , m_pointSize(64)
    , m_verticalSpeed(0.0f)
    , m_touchCounter(0)
    , m_program(0)
{ 
}

Player::~Player()
{

}

void Player::OnInit(int width, int height) 
{
    LOG_INFO("Player initialization");
    
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_program = OGLHelper::CreateProgram(vertexShader, fragmentShader);
    if (!m_program) 
    {
        LOG_ERROR("Could not create program for the Player class.");
        return;
    }
    m_positionHandle = glGetAttribLocation(m_program, "positionVec");
    LOG_GL_ERRORS("glGetAttribLocation");
    LOG_INFO("glGetAttribLocation(\"positionVec\") = %d\n", m_positionHandle);

    m_pointSizeHandle = glGetUniformLocation(m_program, "pointSize");
    LOG_GL_ERRORS("glGetUniformLocation");
    LOG_INFO("glGetUniformLocation(\"pointSize\") = %d\n", m_pointSizeHandle);

    m_pointSize = std::min(width, height) * POINT_RELATIVE_SIZE;
}

void Player::OnDraw() 
{
    LOG_INFO("Player OnDraw method");

    if(!m_program)
    {
        LOG_ERROR("OpenGl program is not initialized for Player class");
        return;
    }

    glUseProgram(m_program);
    LOG_GL_ERRORS("glUseProgram");

    glVertexAttribPointer(m_positionHandle, 2, GL_FLOAT, GL_FALSE, 0, &m_position);
    LOG_GL_ERRORS("glVertexAttribPointer");
    glEnableVertexAttribArray(m_positionHandle);
    LOG_GL_ERRORS("glEnableVertexAttribArray");

    glUniform1f(m_pointSizeHandle, m_pointSize);

    glDrawArrays(GL_POINTS, 0, 1);
    LOG_GL_ERRORS("glDrawArrays");
}

void Player::OnTouch(float xPos, float yPos) 
{
    ++m_touchCounter;
    m_verticalSpeed = 0.0f;
}

void Player::OnStop() 
{
    m_position = positionInitialValues;
    m_verticalSpeed = 0.0f;
    m_touchCounter = 0;
}

void Player::Iterate(double timeElapsed) 
{
    LOG_INFO("Player Iterate method");
   
    m_previousPosition.y = m_position.y;   // no need to save prev X position, it will be not changed

    m_verticalSpeed += timeElapsed * VERTICAL_ACCELERATION - m_touchCounter * TOUCH_ACCELERATION;
    m_position.y += (-m_verticalSpeed) * timeElapsed;
    m_touchCounter = 0;
}

void Player::SetPosition(Point pos)
{
    m_previousPosition = m_position;   //save both positions, because X could be changed

    m_position = pos;
}

GLfloat Player::GetHorizontalSpeed() const
{
    return HORIZONTAL_SPEED;
}

const Point &Player::GetPosition() const
{
    return m_position;
}

const Point &Player::GetPreviousPosition() const
{
    return m_previousPosition;
}

GLfloat Player::GetRadius() const
{
    return m_pointSize / 2.0f;
}