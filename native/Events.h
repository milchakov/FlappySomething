#pragma once

#ifndef events_h
#define events_h

#include <stddef.h>

class GameEvents
{
public:

    virtual void OnInit(int width, int height) {};
    virtual void OnDraw() {};
    virtual void OnTouch(float xPos, float yPos) {};
    virtual void OnStop() {};
};

#endif //events_h