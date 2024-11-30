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
		PlayState(GameStateManager& stateManager,const std::string& randomWord, Subjects subject);
		~PlayState() override {};
		
		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render() override;

		int doesLetterExist(char letter) const;

	private:
		void FillLetterBank();
		void RenderLinePerLetter();
		void SetStickman();
		void ResetGame();
		void ProcessLetterButtonsInput();
		void RenderHangman();
		void RenderCorrectLetters();

	private:
		GameStateManager& 	m_GameStateManager;
		std::string 		m_Word;
		Subjects 			m_ECurrentSubject;
		std::string 		m_CurrentSubjectName;
		const int 			MAX_WRONG_GUESSES = 6;
		Timer 				m_Timer;
		int 				m_WrongGuesses;
		int 				m_Score;
		bool				m_Won;
		bool				m_Lost;
		bool 				m_SpaceRemoved;
		UniqueTextRenderer 	m_WordRenderer;
		UniqueTextRenderer 	m_StickmanRenderer;
		UniqueButton 		m_NextWordButton;
		UniqueButton 		m_BackToSubjectsButton;
		UniqueButton 		m_QuitButton;
		std::vector<Button> m_LettersButtons;
		std::vector<char> 	m_CorrectLetters; 

		std::unordered_map<char, std::vector<glm::ivec2>> m_LetterToLineMap;
		std::vector<std::pair<char, glm::ivec2>> m_StickmanParts;
		std::vector<std::pair<char, glm::ivec2>> m_PoleParts;
	};
}