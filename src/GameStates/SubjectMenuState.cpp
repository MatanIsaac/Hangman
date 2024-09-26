#include "SubjectMenuState.hpp"
#include "Game.hpp"
#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp" 
#include "GameStates/PlayState.hpp"
#include <iostream>

SubjectMenuState::SubjectMenuState(Game* game)
    : m_Game(game)
{
    std::cout << "SubjectMenu CTOR\n";

    m_CurrentSubject = Subject::Subjects::NONE;

	glm::vec2 foodButtonPosition( glm::vec2( SCREEN_WIDTH / 3.f, ( SCREEN_HEIGHT / 3.f ) ) );
	m_SubjectFood.reset(new Button(game->GetRenderer(), "FOOD", 36, foodButtonPosition));

    m_TextRenderer.reset(new TextRenderer
	(
		game->GetRenderer(),
		"Assets\\fonts\\Filmcryptic.ttf",
		40
	));
}

SubjectMenuState::~SubjectMenuState()
{
    std::cout << "SubjectMenu DTOR\n";
    Clean();
}

void SubjectMenuState::ProcessInput()
{
    m_SubjectFood->ProcessInput();
	if (m_SubjectFood)
	{
		if (m_SubjectFood->isButtonPressed() && !m_SubjectFood->GetButtonLocked())
		{
			m_SubjectFood->SetButtonLock(true);
		}
		else if (!m_SubjectFood->isButtonPressed() && m_SubjectFood->GetButtonLocked())
		{
			m_SubjectFood->SetButtonLock(false);
            m_CurrentSubject = Subject::Subjects::FOOD;
			std::string randomWord = Subject::GetRandomWord(m_CurrentSubject);
            PlayState* playState = new PlayState(m_Game, randomWord, m_CurrentSubject);
			m_Game->ChangeState(GameStateType::PLAY,playState);
		}
	}
}

void SubjectMenuState::Update( float deltaTime )
{

}

void SubjectMenuState::Render( SDL_Renderer* renderer )
{
    m_SubjectFood->Render();
    m_TextRenderer->RenderText
    (
        250, 
        100,
        COLOR_LIGHTORANGE,
        "Pick a Subject:"
    );
}

void SubjectMenuState::Clean()
{
    delete m_Game; 
}