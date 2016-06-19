#pragma once

#ifndef menu_h
#define menu_h

#include "GameObjectBase.h"
#include "Utils.h"

#include <string>
#include <array>

/*
Simple menu class which represents simple texture
which draws in center of the display with or without scale.
*/
class TexturedMenu : public GameObjectBase
{
public:

    explicit TexturedMenu(const char* imageName);
    ~TexturedMenu();

    void OnInit(int width, int height) override;
    void OnDraw() override;
    void OnTouch(float xPos, float yPos) override;

private:

    // init position array with scale
    void InitPositionArray(int texWidth, int texheight, int dispWidth, int dispHeight);

    const std::string m_imageName;

    GLuint m_program;
    GLuint m_positionHandle;
    GLuint m_textureCoordsHandle;
    GLuint m_textureDataHandle;

    GLuint m_textureId;

    std::array <Point, 4> m_positionArray;    // 8 - four 2d coordinate

};

#endif  //menu_h