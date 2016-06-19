#pragma once

#ifndef player_h
#define player_h

#include "GameWorldItemBase.h"
#include "Utils.h"

/*
Player represents solid circle which falls to the ground with VERTICAL_ACCELERATION.
Every touch on the screen causes reset vertical speed to zero 
and decrement it with TOUCH_ACCELERATION after.
Technically player realized by using gl_PointSize.
*/
class Player : public GameWorldItemBase
{
public:

    Player();
    ~Player();

    void OnInit(int width, int height) override;
    void OnDraw() override;
    void OnTouch(float xPos, float yPos) override;
    void OnStop() override;

    void Iterate(double timeElapsed) override;

    void SetPosition(Point pos);

    GLfloat GetHorizontalSpeed() const;
    const Point &GetPosition() const;
    const Point &GetPreviousPosition() const;
    GLfloat GetRadius() const;

private:

    static constexpr GLfloat VERTICAL_ACCELERATION = 1.0f;    
    static constexpr GLfloat TOUCH_ACCELERATION = 0.6f;
    static constexpr GLfloat POINT_RELATIVE_SIZE = 0.06f;
    static constexpr GLfloat HORIZONTAL_SPEED = 0.2f;

    Point m_position;
    Point m_previousPosition;
    GLfloat m_pointSize;

    GLfloat m_verticalSpeed;

    unsigned int m_touchCounter;

    GLuint m_program;
    GLuint m_positionHandle;
    GLuint m_pointSizeHandle;
};

#endif  //player_h