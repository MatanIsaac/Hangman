#include "MenuState.hpp"
#include "Game.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"

#include <glm/glm.hpp>
#include <iostream>


MenuState::MenuState(Game* game)
	: m_Game(game)
{
	float xAxisCenter = SCREEN_WIDTH / 2.f;

	glm::vec2 playButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) ) );
	m_PlayButton.reset(new Button(m_Game->GetRenderer(), "Play", 36, playButtonPosition));
	
	glm::vec2 quitButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) + 50.f ) );
	m_QuitButton.reset(new Button(m_Game->GetRenderer(), "Quit", 36, quitButtonPosition)); 

	m_TextRenderer.reset(new TextRenderer 
	(
		game->GetRenderer(),
		"Assets/fonts/Filmcryptic.ttf",
		40
	));
}

MenuState::~MenuState()
{
	Clean();
}

void MenuState::ProcessInput()
{
	m_PlayButton->ProcessInput();
	if (m_PlayButton->isPressed())
	{
		m_Game->ChangeState(GameStateType::SUBJECT);
	}

	m_QuitButton->ProcessInput();
	if (m_QuitButton->isPressed())
	{
		m_Game->QuitGame();	   
	}
}

void MenuState::Update( float deltaTime )
{
	
}

void MenuState::Render( SDL_Renderer* renderer )
{
	m_PlayButton->Render();
	m_QuitButton->Render();
	m_TextRenderer->RenderText(250, 100,COLOR_LIGHTORANGE,"Welcome To Hangman");

}

void MenuState::Clean()
{
	m_PlayButton->Clean();
	m_QuitButton->Clean();
}

