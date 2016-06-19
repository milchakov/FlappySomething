#include "FlappySomething.h"

#include "GameWorld.h"
#include "Menu.h"
#include "Utils.h"



FlappySomething::FlappySomething()
    : m_currentGameState(GS_START_STATE)
{
    m_gameObjects[GS_START_STATE].reset(new TexturedMenu("start_screen.png"));
    m_gameObjects[GS_RUN_STATE].reset(new GameWorld());
    m_gameObjects[GS_GAME_OVER_STATE].reset(new TexturedMenu("game_over_screen.png"));
}

FlappySomething::~FlappySomething()
{
    LOG_INFO("FlappySomething::~FlappySomething destruction");
}

void FlappySomething::OnInit(int width, int height)
{
    LOG_INFO("setupGraphics(%d, %d)", width, height);

    for(auto &gameObject : m_gameObjects)
    {
        gameObject->OnInit(width, height);
    }

    glViewport(0, 0, width, height);
    LOG_GL_ERRORS("glViewport");
}

void FlappySomething::OnDraw()
{
    m_gameObjects[m_currentGameState]->OnDraw();
}

void FlappySomething::OnTouch(float xPos, float yPos)
{
    m_gameObjects[m_currentGameState]->OnTouch(xPos, yPos);
}

void FlappySomething::OnStop()
{
    if(GS_RUN_STATE == m_currentGameState)
    {
        m_currentGameState = GS_GAME_OVER_STATE;
        m_gameObjects[GS_RUN_STATE]->OnStop();
    }
}

void FlappySomething::SwitchToNextState()  
{
    switch (m_currentGameState)     
    {
    case GS_START_STATE: m_currentGameState = GS_RUN_STATE;
        break;
    case GS_RUN_STATE: m_currentGameState = GS_GAME_OVER_STATE;
        break;
    case GS_GAME_OVER_STATE: m_currentGameState = GS_RUN_STATE;
    default: 
        m_currentGameState = GS_RUN_STATE;
    }
}

void FlappySomething::InitTextureLoader(JNIEnv* env, jobject bitmapLoader)
{
    m_textureLoader.Init(env, bitmapLoader);
}


bool FlappySomething::LoadTexture(Texture &texture, const char *path) const
{
    return m_textureLoader.LoadTexture(texture, path);
}