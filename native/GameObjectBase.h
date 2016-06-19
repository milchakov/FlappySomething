#pragma once

#ifndef game_object_base_h
#define game_object_base_h

#include "Events.h"


class GameObjectBase : public GameEvents
{
public:

    GameObjectBase() {};
    virtual ~GameObjectBase() = 0;

    // non-copyable
    GameObjectBase(const GameObjectBase&) = delete;
    GameObjectBase & operator =(const GameObjectBase&) = delete;
};


#endif  //game_object_base_h