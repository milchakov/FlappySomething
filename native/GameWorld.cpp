#include "GameWorld.h"

#include "Player.h"
#include "Pipes.h"
#include "Utils.h"
#include "Collider.h"
#include "FlappySomething.h"

namespace //anonymous namespace, helpers
{

    template <class container, class fn>
    void ForEach(const container &cont, fn func)
    {
        for(auto &item_ptr : cont)
        {
            func(*item_ptr);
        }
    }

}   //anonymous namespace, helpers


namespace GWConstants   // game world constants
{
    constexpr double STEP_TIME = 0.01;
    constexpr double MAX_FRAME_TIME = 0.25;
    constexpr float WAIT_AFTER_COLLISION = 1.0f;

}   // game world constants

GameWorld::GameWorld()
    : m_width(0)
    , m_height(0)
    , m_stepAccumulator(0.0)
    , m_collisionDetected(false)
    , m_score(0)
{
    std::unique_ptr <Player> playerPtr(new Player());

    m_gameWorldItems[PIPES_ITEM].reset( new Pipes(playerPtr->GetHorizontalSpeed()) );   // need to use speed of player for pipes moving

    m_gameWorldItems[PLAYER_ITEM].reset( playerPtr.release() );     // pointer ownership moving
}

GameWorld::~GameWorld()
{
}

void GameWorld::OnInit(int width, int height) 
{
    m_width = width;
    m_height = height;
    m_previousTime = std::chrono::high_resolution_clock::now();

    LOG_INFO("GameWorld initialization");
    ForEach( m_gameWorldItems, std::bind(&GameObjectBase::OnInit, std::placeholders::_1, m_width, m_height));
}

void GameWorld::OnDraw()  
{
    LOG_INFO("GameWorld OnDraw method");

    IterateItems();

    glClearColor(0.6f, 0.6f, 0.6f, 1.0f);  
    LOG_GL_ERRORS("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    LOG_GL_ERRORS("glClear");
    
    ForEach(m_gameWorldItems, std::bind(&GameWorldItemBase::OnDraw, std::placeholders::_1));

    if(m_collisionDetected)
    {
        WaitAndSwitchOnCollision();
    }
}

void GameWorld::OnTouch(float xPos, float yPos)
{
    ForEach(m_gameWorldItems, std::bind(&GameWorldItemBase::OnTouch, std::placeholders::_1, xPos, yPos));
}

void GameWorld::OnStop()
{
    ForEach(m_gameWorldItems, std::bind(&GameWorldItemBase::OnStop, std::placeholders::_1));
    m_collisionDetected = false;
    m_stepAccumulator = 0.0f;
}

void GameWorld::IterateItems()
{
    using namespace std::chrono;

    if(m_collisionDetected)
    {
        return;
    }

    high_resolution_clock::time_point currentTime = high_resolution_clock::now(); 
    duration<double> frameTime = duration_cast<duration<double>>(currentTime - m_previousTime);
    if ( frameTime.count() > GWConstants::MAX_FRAME_TIME ) 
    {
        m_stepAccumulator += GWConstants::MAX_FRAME_TIME; 
    }
    else
    {
        m_stepAccumulator += frameTime.count();
    }

    m_previousTime = currentTime;  

    while ( m_stepAccumulator >= GWConstants::STEP_TIME )   // iterate with fixed time-step
    { 
        ForEach(m_gameWorldItems, std::bind(&GameWorldItemBase::Iterate, std::placeholders::_1, GWConstants::STEP_TIME));

        Player &player = static_cast <Player&> (*(m_gameWorldItems[PLAYER_ITEM].get()));    // do not need to use dynamic cast
        Pipes &pipes = static_cast <Pipes&> (*(m_gameWorldItems[PIPES_ITEM].get()));        // because we exactly know the type

        if(Collider::CheckIncrementScore(m_score, player, pipes))
        {
            ++m_score;
        }

        if(Collider::CheckAndSolveCollisions(player, pipes, m_width, m_height))
        {
            m_collisionDetected = true;
        }

        m_stepAccumulator -= GWConstants::STEP_TIME; 
    } 
}

//draw last display some time after collision
 void GameWorld::WaitAndSwitchOnCollision()
 {
     using namespace std::chrono;

     high_resolution_clock::time_point currentTime = high_resolution_clock::now(); 
     duration<double> frameTime = duration_cast<duration<double>>(currentTime - m_previousTime);

     if(frameTime.count() > GWConstants::WAIT_AFTER_COLLISION)
     {
         FlappySomethingEngine::Instance().SwitchToNextState();
         OnStop();
     }
 }