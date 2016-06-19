#pragma  once

#ifndef collider_h
#define collider_h

#include "Player.h"
#include "Pipes.h"
#include "Utils.h"


namespace   //anonymous namespace, helpers
{
    bool IsZero(float val)
    {
        static float eps = 1e-10;
        return (val < eps && val > (0 - eps));
    }

    struct Line
    {
        Line()
        {}

        Line(const Point &p1, const Point &p2)
            : A(p1)
            , B(p2)
        {}

        operator bool()
        {
            return !(IsZero(A.x) && IsZero(A.y) && IsZero(B.x) && IsZero(B.y));
        }

        Point A;
        Point B;
    };

}   //anonymous namespace, helpers   


/*
Static class for collision detection and solving.
Intersection detection based on line and circle equations.
*/
class Collider
{
public:

    static bool CheckAndSolveCollisions(Player &player, const Pipes &pipes, int width, int height);
    static bool CheckIncrementScore(unsigned int currentScore, const Player &player, const Pipes &pipes);

private:

    static bool CheckAndSolveInternal(
                                        Player &player,
                                        GLfloat radius,
                                        const Point &playerPos, 
                                        const std::vector <Line> &lines, 
                                        const Pipes::OffsetsArray &offsets);

    static bool CheckLineIntersection(const Point& circlePoint, GLfloat radius, const Line &line);
    static void SolveCollision(Player &player, GLfloat radius, const Point &playerPos, const Point &offset, const Line &line);
};

#endif  //collider_h