#include "MenuState.hpp"
#include "Game.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"

#include <glm/glm.hpp>
#include <iostream>

MenuState::MenuState(Game* game)
	: m_Game(game)
{
	std::cout << "Menu CTOR\n";
}

MenuState::~MenuState()
{
	std::cout << "Menu DTOR\n";
	Clean();
}

void MenuState::ProcessInput()
{

}

void MenuState::Update( float deltaTime )
{
	
}

void MenuState::Render( SDL_Renderer* renderer )
{
}

void MenuState::Clean()
{
	delete m_Game;
}

