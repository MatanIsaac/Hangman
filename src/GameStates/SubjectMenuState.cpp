#include "SubjectMenuState.hpp"
#include "Game.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp" 
#include "GameStates/PlayState.hpp"
#include <iostream>

SubjectMenuState::SubjectMenuState(Game* game)
    : m_Game(game)
{
    std::cout << "Menu CTOR\n";
}

SubjectMenuState::~SubjectMenuState()
{
    std::cout << "Menu DTOR\n";
    Clean();
}

void SubjectMenuState::ProcessInput()
{
    
}

void SubjectMenuState::Update( float deltaTime )
{

}

void SubjectMenuState::Render( SDL_Renderer* renderer )
{

}

void SubjectMenuState::Clean()
{
    delete m_Game; 
}