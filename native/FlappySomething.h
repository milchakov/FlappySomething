#pragma once

#ifndef flappy_something_h
#define flappy_something_h

#include "GameObjectBase.h"
#include "Singleton.h"
#include "TextureLoader.h"

#include <memory>
#include <array>

class FlappySomething;
using FlappySomethingEngine = Singleton <FlappySomething>;


enum GameState
{
    GS_START_STATE,
    GS_RUN_STATE,
    GS_GAME_OVER_STATE,

    GS_COUNT_STATES // should be last element in enum
};

using GameObjectsArray = std::array <std::unique_ptr<GameObjectBase>, GS_COUNT_STATES>; 

/*
Main class of the native part of application, 
contains all elements of application (two menu and game world)
*/
class FlappySomething : public GameObjectBase
{
public:
    FlappySomething();
    ~FlappySomething();

    void OnInit(int width, int height) override;
    void OnDraw() override;
    void OnTouch(float xPos, float yPos) override;
    void OnStop() override;

    void InitTextureLoader(JNIEnv* env, jobject bitmapLoader);
    bool LoadTexture(Texture &texture, const char *path) const;

    void SwitchToNextState();

private:

    TextureLoader m_textureLoader;

    GameState m_currentGameState;   
    GameObjectsArray m_gameObjects;
};



#endif  //flappy_something_h