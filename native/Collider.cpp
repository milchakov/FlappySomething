#include "Collider.h"

#include "Player.h"
#include "Pipes.h"
#include "Utils.h"

#include <vector>
#include <cmath>

namespace   //anonymous namespace, helpers
{
    /*
    Helper class which using for convert relative coordinates to absolute
    */
    class PointConvertHelper
    {
    public:
        PointConvertHelper()
            : m_halfWidth(1e-16)   // exclude division by zero if not initialized
            , m_halfHeight(1e-16)
        {}

        void Init(int width, int height)
        {
            m_halfWidth = (GLfloat)width / 2.f;
            m_halfHeight = (GLfloat)height / 2.f;
        }

        Point ToRrelativePoint(GLfloat xPos, GLfloat yPos) const
        {
            return Point((xPos - m_halfWidth) / m_halfWidth, (yPos - m_halfHeight) / m_halfHeight);
        }

        Point ToAbsPoint(const Point &pt) const
        {
            return Point(m_halfWidth + pt.x * m_halfWidth, m_halfHeight + pt.y * m_halfHeight);
        };

        GLfloat ToAbsWidth(GLfloat relatiteWidth) const
        {
            return relatiteWidth * m_halfWidth;
        }

        GLfloat ToAbsHeight(GLfloat relativeHeight) const
        {
            return relativeHeight * m_halfHeight;
        }

        GLfloat GetHalfWidth() const
        {
            return m_halfWidth;
        }

        GLfloat GetHalfHeight() const 
        {
            return m_halfHeight;
        }

    private:

        GLfloat m_halfWidth;
        GLfloat m_halfHeight;
    };


    PointConvertHelper pc;  // static helper

    bool EqualFloat(float lhs, float rhs)
    {
        static float eps = 1e-10;
        return fabs(lhs - rhs) < eps;
    }

}   //anonymous namespace, helpers 



/*static*/ bool Collider::CheckAndSolveCollisions(Player &player, const Pipes &pipes, int width, int height)
{
    LOG_INFO("CheckAndSolveCollisions");

    pc.Init(width, height);

    auto pipesCoords = pipes.GetTriangleCoords();
    auto playerPos = player.GetPosition();
    auto radius = player.GetRadius();

    playerPos = pc.ToAbsPoint(playerPos);

    if(playerPos.y - radius <= 0.0f || playerPos.y + radius >= height)    // solve collision with top and bottom
    {
        playerPos.y = (playerPos.y - radius < 0.0f) ? 
                                            radius : 
                                            (height - radius);

        player.SetPosition(pc.ToRrelativePoint(playerPos.x, playerPos.y));
        LOG_INFO("CollisionSolver - collision detected with top or bottom");
        return true;
    }

    auto MakeLine = [&pipesCoords](size_t index) -> Line
    {
        return Line(
            pc.ToAbsPoint(pipesCoords[index]), 
            pc.ToAbsPoint(pipesCoords[index + 1]));
    };

    std::vector <Line> lines;   // make lines just for first figure and next work with offsets.
    for(size_t i = 0; i + 3 < pipesCoords.size(); i += Pipes::FIGURE_CONTAINS / Pipes::VERTEX_SIZE / 2)  // 2 - count rects in figure
    {
        lines.emplace_back(MakeLine(i));
        lines.emplace_back(MakeLine(i + 1));
    }

    return CheckAndSolveInternal(player, radius, playerPos, lines, pipes.GetOffsets());
}



/*static*/ bool Collider::CheckIncrementScore(unsigned int currentScore, const Player &player, const Pipes &pipes)
{
    auto pipesOffsets = pipes.GetOffsets();
    auto playerPosition = player.GetPosition();

    return playerPosition.x > pipesOffsets[currentScore % Pipes::FIGURES_COUNT].x;
}



/*static*/ bool Collider::CheckAndSolveInternal(
    Player &player,
    GLfloat radius,
    const Point &playerPos, 
    const std::vector <Line> &lines, 
    const Pipes::OffsetsArray &offsets)
{

    for(size_t i = 0; i < offsets.size(); ++i)
    {
        Point movedPosition(
            playerPos.x - pc.ToAbsWidth(offsets[i].x), 
            playerPos.y - pc.ToAbsHeight(offsets[i].y));

        for(size_t j = 0; j + 1 < lines.size(); j += 2)
        {
            Line lineForCheck;
            bool collideFirst = CheckLineIntersection(movedPosition, radius, lines[j]);
            bool collideSecond = CheckLineIntersection(movedPosition, radius, lines[j + 1]);

            if(collideFirst && collideSecond)   // tricky case, just move to the previous state
            {
                player.SetPosition(player.GetPreviousPosition());
                return true;
            }
            else if(collideFirst)
            {
                lineForCheck = lines[j];
            }
            else if(collideSecond)
            {
                lineForCheck = lines[j + 1];
            }

            if(collideFirst || collideSecond) 
            {
                if(lineForCheck)
                {                   
                    SolveCollision(player, radius, playerPos, offsets[i], lineForCheck);
                }
                return true;
            }
        }
    }
    return false;
}



/*static*/ bool Collider::CheckLineIntersection(const Point& circlePoint, GLfloat radius, const Line &line)
{
    LOG_INFO("CheckLineIntersection circlePoint(%f,%f), radius = %f, line((%f,%f);(%f,%f))", 
        circlePoint.x, circlePoint.y, radius, line.A.x, line.A.y, line.B.x, line.B.y);

    // use system of coordinate with circlePoint in 0,0
    double diffACircleX = line.A.x - circlePoint.x;
    double diffACircleY = line.A.y - circlePoint.y;
    double diffBCircleX = line.B.x - circlePoint.x;
    double diffBCircleY = line.B.y - circlePoint.y;

    double diffX = diffBCircleX - diffACircleX;
    double diffY = diffBCircleY - diffACircleY;

    // quadratic equation (calc discriminant and roots)
    double a = diffX * diffX + diffY * diffY;
    double b = 2 * ((diffX * diffACircleX) + (diffY * diffACircleY));
    double c = (diffACircleX * diffACircleX) + (diffACircleY * diffACircleY) - (radius * radius);

    double d = b * b - (4 * a * c);  

    LOG_INFO("CheckLineIntersection a = %f; b = %f; c = %f; d = %f", a, b, c, d);

    if(d < 0)
    {
        return false;
    }
    d = sqrt(d);
    double root1 = (-b + d) / (2 * a);
    double root2 = (-b - d) / (2 * a);

    return ((root1 >= 0.0) && (root1 <= 1.0)) || ((root2 >= 0.0) && (root2 <= 1.0));
}



/*static*/ void Collider::SolveCollision(Player &player, GLfloat radius, const Point &playerPos, const Point &offset, const Line &line)
{
    if(EqualFloat(line.A.x, line.B.x))        // vertical line
    {
        player.SetPosition(
            pc.ToRrelativePoint(line.A.x + pc.ToAbsWidth(offset.x ) - radius,         // just move circle to left
            playerPos.y)); 
    }
    else if(EqualFloat(line.A.y, line.B.y))   // horizontal line
    {
        Point prevPos = player.GetPreviousPosition();

        if(line.A.y > (pc.GetHalfHeight() + pc.ToAbsHeight(prevPos.y) - pc.ToAbsHeight(offset.y)))    // check line was upper or lower than player (move direction)
        {
            player.SetPosition(pc.ToRrelativePoint(
                playerPos.x, 
                line.A.y + pc.ToAbsHeight(offset.y) - radius)); // just move circle to bottom
        }
        else
        {
            player.SetPosition(pc.ToRrelativePoint(
                playerPos.x, 
                line.A.y + pc.ToAbsHeight(offset.y) + radius)); // just move circle to top
        }
    }
}