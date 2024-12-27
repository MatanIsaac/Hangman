#include "MenuState.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"
#include "SubjectMenuState.hpp"
#include "Core/GameStateManager.hpp"
#include <SDL2/SDL_mixer.h>
#include <glm/glm.hpp>
#include <iostream>

namespace isaac_hangman
{

	MenuState::MenuState(IGame& game)
		: m_Game(game)
	{
		m_StateName = "MenuState";

		float xAxisCenter = SCREEN_WIDTH / 2.f;

		glm::vec2 playButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) ) );
		m_PlayButton = std::make_unique<Button>("Play", playButtonPosition);
		
		glm::vec2 quitButtonPosition( glm::vec2( xAxisCenter - 25, ( SCREEN_HEIGHT / 3.f ) + 50.f ) );
		m_QuitButton = std::make_unique<Button>("Quit", quitButtonPosition); 
				
	}

	void MenuState::ProcessInput()
	{	
		if (m_PlayButton->isPressed())
		{
			GameStateManager::GetInstance()->PushState(std::make_shared<SubjectMenuState>(m_Game));
		}

		if (m_QuitButton->isPressed())
		{
			m_Game.Quit();
		}
	}

	void MenuState::Update( float deltaTime )
	{
		m_PlayButton->Update(deltaTime);
		m_QuitButton->Update(deltaTime);
	}

	void MenuState::Render(SDL_Renderer* renderer)
	{
		m_PlayButton->Render(renderer);
		m_QuitButton->Render(renderer);

		auto& textRenderer = TextRenderer::GetInstance();
		textRenderer.RenderText(renderer,250, 100,COLOR_LIGHTORANGE,"Welcome To Hangman");
	}
}