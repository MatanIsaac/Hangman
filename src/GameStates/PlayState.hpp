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
#include "SubjectMenuState.hpp"
#include "Core/GameStateManager.hpp"
#include "Util/Timer.hpp"
#include "Core/IGame.hpp"

namespace isaac_hangman
{
	class PlayState : public IGameState
	{
	public:
		PlayState(GameStateManager& stateManager,const std::string& randomWord, Subjects subject, IGame& game);
		virtual ~PlayState() override {}
		
		void ProcessInput() override;
		void Update( float deltaTime ) override;
		void Render(SDL_Renderer* renderer) override;

		int doesLetterExist(char letter) const;

	private:
		void FillLetterBank();
		void RenderLinePerLetter(SDL_Renderer* renderer);
		void ResetGame();
		void ProcessLetterButtonsInput();
		void RenderHangman(SDL_Renderer* renderer);
		void RenderCorrectLetters(SDL_Renderer* renderer);

	private:
		GameStateManager& 	m_GameStateManager;
		std::string 		m_Word;
		Subjects 			m_ECurrentSubject;
		IGame& 				m_Game; 
		int 				m_WrongGuesses;
		int 				m_Score;
		bool				m_Won;
		bool				m_Lost;
		const int 			MAX_WRONG_GUESSES = 6;
		bool 				m_SpaceRemoved;
		std::string 		m_CurrentSubjectName;
		Unique_Button 		m_NextWordButton;
		Unique_Button 		m_BackToSubjectsButton;
		Unique_Button 		m_QuitButton;
		std::vector<Button> m_LettersButtons;
		std::vector<char> 	m_CorrectLetters; 
		std::unordered_map<char, std::vector<glm::ivec2>> m_LetterToLineMap;
	};
}