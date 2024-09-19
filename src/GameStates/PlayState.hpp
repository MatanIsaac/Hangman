#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

#include "IGameState.hpp"

class Game;

class PlayState : public IGameState
{
public:
	PlayState(Game* game, const std::string& randomWord);
	~PlayState() override;
	
	void ProcessInput() override;
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer ) override;
	void Clean() override;

	bool doesLetterExist(char letter) const;

private:
	void FillLetterBank();
	void RenderLinePerLetter(SDL_Renderer* renderer);

private:
	Game* m_Game;

};
