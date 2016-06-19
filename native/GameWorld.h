#pragma once

#ifndef game_world_h
#define game_world_h

#include "GameObjectBase.h"
#include "GameWorldItemBase.h"

#include <memory>
#include <array>
#include <chrono>
 
/*
Class which represents game world and 
contains all other elements of the game (player and pipes)
*/
class GameWorld : public GameObjectBase
{
public:

    GameWorld();
    ~GameWorld();

    void OnInit(int width, int height) override;
    void OnDraw() override;
    void OnTouch(float xPos, float yPos) override;
    void OnStop() override;

private:

    void IterateItems();
    void WaitAndSwitchOnCollision();    //draw last display some time after collision

    int m_width;
    int m_height;

    std::chrono::high_resolution_clock::time_point m_previousTime;  // time step counters
    double m_stepAccumulator;

    bool m_collisionDetected;
    unsigned int m_score;       // just counter without drawing

    enum GameWorldItemKey
    {
        PIPES_ITEM,
        PLAYER_ITEM,

        COUNT_ITEMS  // should be last element in enum 
    };

    using GameWorldItemsArray = std::array <std::unique_ptr<GameWorldItemBase>, COUNT_ITEMS>;

    GameWorldItemsArray m_gameWorldItems;
};

#endif  //game_world_h