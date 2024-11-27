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
#include "Core/GameStateManager.hpp"
#include "Util/Timer.hpp"

namespace isaac_hangman
{
		
	class PlayState : public IGameState
	{
	public:
		PlayState(GameStateManager& stateManager,const std::string& randomWord, Subject::Subjects subject);
		~PlayState() override {};
		
		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render() override;

		int doesLetterExist(char letter) const;

	private:
		void FillLetterBank();
		void RenderLinePerLetter();
		void SetStickman();
	private:
		GameStateManager& m_GameStateManager;
		std::string m_Word;
		bool m_Lose;
		bool m_SpaceRemoved;
		int m_WrongGuesses;
		const int MAX_WRONG_GUESSES = 6;
		std::vector<Button*> m_LettersButtons;
		// Using 2 text renderers to render the word and the stickman parts 
		// For renderer with different font sizes
		std::unique_ptr<TextRenderer> m_WordRenderer;
		std::unique_ptr<TextRenderer> m_PartsRenderer;
		std::unordered_map<char, std::vector<glm::ivec2>> m_LetterToLineMap;
		std::vector<char> m_WordChars; 
		std::vector<std::pair<char, glm::ivec2>> m_StickmanParts;
		std::vector<std::pair<char, glm::ivec2>> m_PoleParts;
		std::unique_ptr<Button> m_NextWordButton;
		std::unique_ptr<Button> m_BackToSubjectsButton;
		std::unique_ptr<Button> m_QuitButton;
		Subject::Subjects m_CurrentSubject;
		Timer m_Timer;
	};
}