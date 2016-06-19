#pragma once

#ifndef game_world_item_base_h
#define game_world_item_base_h

#include "GameObjectBase.h"

class Iterable
{
public:
    virtual void Iterate(double timeElapsed) = 0;
};

class GameWorldItemBase : public GameObjectBase, public Iterable
{
};


#endif  //game_world_item_base_h