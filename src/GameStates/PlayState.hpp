#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "IGameState.hpp"
#include "Graphics/TextRenderer.hpp"
#include "Graphics/Button.hpp"

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
	std::string m_Word;
	bool Lose;
	int wrongGuesses;
	const int MAX_WRONG_GUESSES = 6;
	std::vector<Button*> m_LettersButtons;
	std::unique_ptr<TextRenderer,TextRendererDeleter> m_WordRenderer;
	std::unique_ptr<TextRenderer,TextRendererDeleter> m_PartsRenderer;
	std::unordered_map<char, std::vector<glm::ivec2>> m_LetterToLineMap;
	std::vector<char> m_WordChars; 
	std::vector<std::pair<char, glm::ivec2>> m_StickmanParts;
	std::vector<std::pair<char, glm::ivec2>> m_PoleParts;
};
