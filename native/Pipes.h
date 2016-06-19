#pragma once

#ifndef pipes_h
#define pipes_h

#include "GameWorldItemBase.h"
#include "Utils.h"

#include <array>

/*
Vertical pipes which moves with permanent horizontal speed from right to left. 
Pair of pipes (two rects on one vertical line) makes the figure. 
Maximum count of figures on one display: FIGURES_COUNT = 3.
Horizontal offset between figures is permanent.
Vertical offset calculates every time when figure goes out of screen.
Technically pipes realized as one figure which drawn three times with offset.
*/
class Pipes : public GameWorldItemBase
{
public:

    explicit Pipes(GLfloat speed);

    ~Pipes();

    void OnInit(int width, int height) override;
    void OnDraw() override;
    void OnTouch(float xPos, float yPos) override;
    void OnStop() override;

    void Iterate(double timeElapsed) override;

    static constexpr int  FIGURES_COUNT = 3;
    static constexpr int  FIGURE_CONTAINS = 24;
    static constexpr int  VERTEX_SIZE = 2;

    using PipesArray = std::array <Point, Pipes::FIGURE_CONTAINS / Pipes::VERTEX_SIZE>;
    using OffsetsArray = std::array <Point, Pipes::FIGURES_COUNT>;

    const Pipes::PipesArray &GetTriangleCoords() const;
    const Pipes::OffsetsArray &GetOffsets() const;

private:

    void GenerateOffsets();

 
    
    static constexpr GLfloat  FIGURE_WIDTH = 0.3f;
    static constexpr GLfloat  FIGURE_HEIGHT = 0.8f;
    static constexpr GLfloat  FIGURE_GAP = 0.4f;
    static constexpr GLfloat  FIGURE_OFFSET = ((2.0f + FIGURE_WIDTH) / FIGURES_COUNT) - (FIGURE_WIDTH);
    static constexpr GLfloat  NEXT_FIGURE_POS = FIGURE_WIDTH + FIGURE_OFFSET;
    
    PipesArray m_triangelsCoords;
    OffsetsArray m_offsets;
    
    const GLfloat m_speed;  //  count of displays per second

    GLuint m_program;
    GLuint m_positionHandle;
    GLuint m_offsetXHandle;
    GLuint m_offsetYHandle;
};

#endif  //pipes_h