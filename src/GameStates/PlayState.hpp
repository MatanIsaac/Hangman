#pragma once

#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

#include "IGameState.hpp"
#include "Graphics/TextRenderer.hpp"
#include "Graphics/Button.hpp"
#include "../Subjects.hpp"

class Game;

class PlayState : public IGameState
{
public:
	PlayState(Game* game, const std::string& randomWord, Subject::Subjects subject);
	~PlayState() override;
	
	void ProcessInput() override;
	void Update( float deltaTime ) override;
	void Render( SDL_Renderer* renderer ) override;
	void Clean() override;

	int doesLetterExist(char letter) const;

private:
	void FillLetterBank();
	void RenderLinePerLetter(SDL_Renderer* renderer);

private:
	Game* m_Game;
	std::string m_Word;
	bool m_Lose;
	int m_WrongGuesses;
	const int MAX_WRONG_GUESSES = 6;
	std::vector<Button*> m_LettersButtons;
	// Using 2 text renderers to render the word and the stickman parts 
	// With different font sizes
	std::unique_ptr<TextRenderer,TextRendererDeleter> m_WordRenderer;
	std::unique_ptr<TextRenderer,TextRendererDeleter> m_PartsRenderer;
	std::unordered_map<char, std::vector<glm::ivec2>> m_LetterToLineMap;
	std::vector<char> m_WordChars; 
	std::vector<std::pair<char, glm::ivec2>> m_StickmanParts;
	std::vector<std::pair<char, glm::ivec2>> m_PoleParts;
	std::unique_ptr<Button, ButtonDeleter> m_PlayAgainButton;
	std::unique_ptr<Button, ButtonDeleter> m_QuitButton;
	Subject::Subjects m_CurrentSubject;
};
