#pragma once

#include <memory>
#include <SDL.h>

#include "IGameState.hpp"
#include "Graphics/Button.hpp"
#include "Graphics/TextRenderer.hpp"
#include "../Subjects.hpp"

class Game;

class SubjectMenuState : public IGameState
{
public:
    SubjectMenuState(Game* game);
    ~SubjectMenuState() override;

	void ProcessInput() override;
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer ) override;
	void Clean() override;

private:
	Game* m_Game;

	std::unique_ptr<Button,ButtonDeleter> m_SubjectFood;
	std::unique_ptr<TextRenderer,TextRendererDeleter> m_TextRenderer;
	Subject::Subjects m_CurrentSubject;
};
