#include "PlayState.hpp"
#include "Util/ColorMacros.hpp"
#include "Util/Common.hpp"
#include "Core/GameStateManager.hpp"

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace isaac_hangman
{
    PlayState::PlayState(const std::string& randomWord, Subjects subject, IGame& game)
        : m_Word(randomWord)
        , m_ECurrentSubject(subject)
        , m_Game(game)
        , m_WrongGuesses(0)
        , m_Score(0)
        , m_Won(false)
        , m_Lost(false)
        , m_SpaceRemoved(false)
    {
        m_StateName = "PlayState";
        m_CurrentSubjectName    = "Subject: " + SubjectToString(m_ECurrentSubject);

        glm::vec2 nextWordButtonPosition( glm::vec2( SCREEN_WIDTH - 180.f, SCREEN_HEIGHT - 85 )  );
        m_NextWordButton = std::make_unique<Button>("Next Word",nextWordButtonPosition);

        glm::vec2 BackButtonPosition( glm::vec2( SCREEN_WIDTH/3.f, SCREEN_HEIGHT - 85.f )  );
        m_BackToSubjectsButton = std::make_unique<Button>("Back To Subjects", BackButtonPosition);

        glm::vec2 quitButtonPosition( glm::vec2( 35.f, SCREEN_HEIGHT - 85.f ) );
        m_QuitButton = std::make_unique<Button>("Quit", quitButtonPosition); 

        FillLetterBank(); 
       
    }

    void PlayState::ProcessInput()
    {
        if (m_QuitButton->isPressed())
        {
            m_Game.Quit();
        }

        if(m_BackToSubjectsButton->isPressed())
        {
            GameStateManager::GetInstance()->PopState();
        }
        
        if(!m_Lost && !m_Won)
        {
            if(!m_SpaceRemoved && doesLetterExist(' ') == 1)
            { 
                m_CorrectLetters.push_back(' ');
                m_LetterToLineMap.erase(' ');
                m_SpaceRemoved = true;
            }
            
            ProcessLetterButtonsInput();
        }
        else
        {
            if(m_NextWordButton->isPressed())
            {
                if(m_Lost)
                    m_Score = 0;

                ResetGame();
            }
        }    
        
    }

    void PlayState::Update( float deltaTime )
    {
        m_QuitButton->Update(deltaTime);
        m_BackToSubjectsButton->Update(deltaTime);

        if(!m_Lost && !m_Won)
        {
            for (auto& button : m_LettersButtons)
            {
                button.Update(deltaTime);
            }
        }
        else
        {
            m_NextWordButton->Update(deltaTime);
        }

    }

    void PlayState::Render(SDL_Renderer* renderer)
    {        
        auto& textRenderer = TextRenderer::GetInstance();
        
        // Render the current subject
        textRenderer.RenderText(renderer,25, 25, COLOR_LIGHTORANGE, m_CurrentSubjectName);
        
        // Render the score at the top-right corner
        std::string scoreText = "Score: " + std::to_string(m_Score);
        textRenderer.RenderText(renderer,SCREEN_WIDTH - 200, 25, COLOR_LIGHTORANGE, scoreText);
        
        // Render the hangman parts
        RenderHangman(renderer);
        
        // Render the lines for letters
        RenderLinePerLetter(renderer);
        
        // Render the correct letters
        RenderCorrectLetters(renderer);
        
        // Render buttons
        m_QuitButton->Render(renderer);
        m_BackToSubjectsButton->Render(renderer);

        if (!m_Lost && !m_Won)
        {
            for (auto& button : m_LettersButtons)
            {
                button.Render(renderer);
            }
        }
        else
        {
            m_NextWordButton->Render(renderer);

            auto str = (m_Won && !m_Lost) ? "You Won!" : "You Lost!";
            textRenderer.RenderText(renderer,SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 - 125, COLOR_WHITE, str);
            textRenderer.RenderText(renderer,SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, COLOR_WHITE, "Would You Like To Play Again?");
        }
    }

    void PlayState::FillLetterBank()
    {
        int buttonWidth = 30;
        float x         = 200.f;
        float y         = SCREEN_HEIGHT - 200.f;
        int cnt         = 0;

        m_LettersButtons.clear();
        for (char l = 'A'; l <= 'Z'; ++l) 
        {
            if(cnt == 13)
            {
                x = 200.f;
                y += buttonWidth + 20.f;
            }
            m_LettersButtons.emplace_back(&l, glm::vec2(x, y));
            x += buttonWidth;
            cnt++;
        }
    }

    int PlayState::doesLetterExist(char letter) const
    {
        int cnt = 0;
        for(auto& l : m_Word)
        {
            if(l == letter)
            {
                cnt++;
            }
        }
        return cnt;
    }

    void PlayState::RenderLinePerLetter(SDL_Renderer* renderer)
    {
        // Render lines for each letter in m_Word
        int wordLength  = m_Word.length();             
        int startX      = SCREEN_WIDTH / 3 + 50;
        int startY      = SCREEN_HEIGHT / 2 - 100;
        int margin      = 30;
        int spaceCnt    = 0;
        auto& textRenderer = TextRenderer::GetInstance();

        for (int i = 0; i < wordLength; i++)
        {
            auto it = m_LetterToLineMap.find(m_Word[i]);
            if (m_Word[i] == ' ')   // If the letter is a space, render a space
            {
                textRenderer.RenderText(renderer,startX + 5, startY - 60, COLOR_LIGHTORANGE, " ");
                startX += margin;
                spaceCnt++;
                if(spaceCnt == 2)
                {
                    startX = SCREEN_WIDTH / 3 + 150;
                    startY = SCREEN_HEIGHT / 2;
                }
                continue;
            }
            if (it == m_LetterToLineMap.end())
                m_LetterToLineMap[m_Word[i]] = std::vector<glm::ivec2>{glm::ivec2(startX + 5, startY)};
            else
                it->second.push_back(glm::ivec2(startX + 5, startY));
            
            textRenderer.RenderText(renderer,startX + 5, startY - 60, COLOR_LIGHTORANGE, "_");
            startX += margin;
        }
    }

    void PlayState::ResetGame()
    {
        m_CurrentSubjectName = "Subject: " + SubjectToString(m_ECurrentSubject);
        m_Word          = GetRandomWord(m_ECurrentSubject);
        m_WrongGuesses  = 0;
        m_Won           = false;
        m_Lost          = false;
        m_SpaceRemoved  = false;
        m_CorrectLetters.clear();
        FillLetterBank();
        std::cout << "\n\tWord: " << m_Word << '\n';
    }

    void PlayState::ProcessLetterButtonsInput()
    {
        for (auto it = m_LettersButtons.begin(); it != m_LettersButtons.end();)
        {
            if (!it->isPressed())
            {
                ++it;
                continue;
            }

            std::string letter = it->GetButtonText();
            char letterChar = letter[0];
            int letterExists = doesLetterExist(letterChar);

            if (letterExists == 0)
            {
                m_WrongGuesses++;
                it = m_LettersButtons.erase(it);
                continue;
            }

            // Handle letters that exist
            auto letterIt = m_LetterToLineMap.find(letterChar);
            if (letterIt != m_LetterToLineMap.end())
            {
                int occurrences = letterExists;
                for (int i = 0; i < occurrences; ++i)
                {
                    m_CorrectLetters.push_back(letterChar);
                }
                m_LetterToLineMap.erase(letterIt);
            }

            // Remove the button for the processed letter
            it = m_LettersButtons.erase(it);

            // Check if the word is fully guessed
            if (m_CorrectLetters.size() == m_Word.length())
            {
                m_Score++;
                m_Won = true;
                break; // Exit early as the word is complete
            }
        }
    }


    void PlayState::RenderHangman(SDL_Renderer* renderer)
    {

        SDL_SetRenderDrawColor(renderer, COLOR_LIGHTORANGE.r, COLOR_LIGHTORANGE.g, COLOR_LIGHTORANGE.b, COLOR_LIGHTORANGE.a);

        // Base Parts
        SDL_RenderDrawLine(renderer, 50,  320, 150, 320); // Base horizontal line
        SDL_RenderDrawLine(renderer, 100, 320, 100, 80);  // Vertical pole
        SDL_RenderDrawLine(renderer, 100, 80, 200, 80);   // Top horizontal bar
        SDL_RenderDrawLine(renderer, 200, 80, 200, 110);  // Rope

        if (m_WrongGuesses >= 1) // Head
        {
            SDL_Rect head = { 183, 110, 34, 34 }; // Adjusted for proportions and centering
            SDL_RenderDrawRect(renderer, &head);
        }

        if (m_WrongGuesses >= 2) // Body
        {
            SDL_RenderDrawLine(renderer, 200, 144, 200, 220); // Body length adjusted
        }

        if (m_WrongGuesses >= 3) // Left arm
        {
            SDL_RenderDrawLine(renderer, 200, 160, 170, 190); // Slanted left arm
        }

        if (m_WrongGuesses >= 4) // Right arm
        {
            SDL_RenderDrawLine(renderer, 200, 160, 230, 190); // Slanted right arm
        }

        if (m_WrongGuesses >= 5) // Left leg
        {
            SDL_RenderDrawLine(renderer, 200, 220, 180, 270); // Slanted left leg
        }

        if (m_WrongGuesses >= 6) // Right leg
        {
            SDL_RenderDrawLine(renderer, 200, 220, 220, 270); // Slanted right leg

            // Mark game as lost
            m_Lost = true;
        }

    }

    void PlayState::RenderCorrectLetters(SDL_Renderer* renderer)
    {
        auto& textRenderer = TextRenderer::GetInstance();
        
        for(auto& letter : m_CorrectLetters)
        {
            auto it = m_LetterToLineMap.find(letter);
            if (it != m_LetterToLineMap.end())
            {
                for(auto& line : it->second)
                {
                    if(letter == 'I')
                        textRenderer.RenderText(renderer,line.x+6, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                    else
                        textRenderer.RenderText(renderer,line.x, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                }
            }
        }
    }
}