#include "PlayState.hpp"
#include "Util/ColorMacros.hpp"
#include "Util/Common.hpp"
#include "SubjectMenuState.hpp"
#include "Core/GameContext.hpp"

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>

namespace isaac_hangman
{

    PlayState::PlayState(GameStateManager& stateManager,const std::string& randomWord, Subjects subject)
        : 
        m_GameStateManager(stateManager),
        m_Word(randomWord), 
        m_ECurrentSubject(subject),
        m_WrongGuesses(0),
        m_Score(0),
        m_Won(false),
        m_Lost(false),
        m_SpaceRemoved(false),
        m_WordRenderer(std::make_unique<TextRenderer>(35)),
        m_StickmanRenderer(std::make_unique<TextRenderer>(90))
    {
        m_CurrentSubjectName    = "Subject: " + SubjectToString(m_ECurrentSubject);

        glm::vec2 nextWordButtonPosition( glm::vec2( SCREEN_WIDTH - 180.f, SCREEN_HEIGHT - 85 )  );
        m_NextWordButton = std::make_unique<Button>("Next Word",36,nextWordButtonPosition);

        glm::vec2 BackButtonPosition( glm::vec2( SCREEN_WIDTH/3.f, SCREEN_HEIGHT - 85.f )  );
        m_BackToSubjectsButton = std::make_unique<Button>("Back To Subjects", 36, BackButtonPosition);

        glm::vec2 quitButtonPosition( glm::vec2( 35.f, SCREEN_HEIGHT - 85.f ) );
        m_QuitButton = std::make_unique<Button>("Quit", 36, quitButtonPosition); 

        FillLetterBank(); 
        SetStickman();
    }

    void PlayState::ProcessInput()
    {
        if (m_QuitButton->isPressed())
        {
            GameContext::GetInstance().m_IsRunning = false;	
        }

        if(m_BackToSubjectsButton->isPressed())
        {
            m_GameStateManager.PushState(std::make_shared<SubjectMenuState>(m_GameStateManager));
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

    void PlayState::Render()
    {        
        m_WordRenderer->RenderText(25, 25, COLOR_LIGHTORANGE, m_CurrentSubjectName);
        RenderHangman();
        RenderLinePerLetter();
        RenderCorrectLetters();
        m_QuitButton->Render();
        m_BackToSubjectsButton->Render();

        if(!m_Lost && !m_Won)
        {
            for (auto& button : m_LettersButtons)
            {
                button.Render();
            }
        }

        if(m_Lost || m_Won)
        {
            m_NextWordButton->Render();
            auto str = (m_Won && !m_Lost) ? "You Won!" : "You Lost!";
            m_WordRenderer->RenderText(SCREEN_WIDTH / 2 + 50, SCREEN_HEIGHT / 2 - 100,COLOR_LIGHTORANGE, str);
            m_WordRenderer->RenderText(SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 ,COLOR_LIGHTORANGE, "Would You Like To Play Again?");
        }
    }

    // OPTIMIZE: Creating 26 button and emplacing them / inserting into an unordered map/vector 
    // takes about 0.25 seconds on average, slow.  
    void PlayState::FillLetterBank()
    {
        m_Timer.Start();
        
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
            m_LettersButtons.emplace_back(&l, 40, glm::vec2(x, y));
            x += buttonWidth;
            cnt++;
        }

        auto time = m_Timer.Stop();
        std::cout << "Took: " << time << " Seconds\n";
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

    void PlayState::RenderLinePerLetter()
    {
        // Render lines for each letter in m_Word
        int wordLength  = m_Word.length();             
        int startX      = SCREEN_WIDTH / 3 + 50;
        int startY      = SCREEN_HEIGHT / 2 - 100;
        int margin      = 30;
        int spaceCnt    = 0;

        for (int i = 0; i < wordLength; i++)
        {
            auto it = m_LetterToLineMap.find(m_Word[i]);
            if (m_Word[i] == ' ')   // If the letter is a space, render a space
            {
                m_WordRenderer->RenderText(startX + 5, startY - 60, COLOR_LIGHTORANGE, " ");
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
            
            m_WordRenderer->RenderText(startX + 5, startY - 60, COLOR_LIGHTORANGE, "_");
            startX += margin;
        }
    }

    void PlayState::SetStickman()
    {
        m_PoleParts.clear();

        int startX = 10;
        int startY = 100;
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+200)));   
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+175)));   
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+160)));   
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+145)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+130)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+115)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+100)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+75)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+60)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+45)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+30)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY+15)));    
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+20, startY)));    
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+20, startY- 70)));    
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+35, startY- 70)));    
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+50, startY- 70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+65, startY- 70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+80, startY- 70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+95, startY- 70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+110, startY-70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+125, startY-70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+140, startY-70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+155, startY-70)));   
        m_PoleParts.push_back(std::make_pair('_', glm::ivec2(startX+170, startY-70)));   
        m_PoleParts.push_back(std::make_pair('|', glm::ivec2(startX+205, startY)));   

        startX = 190;
        startY = 180;

        m_StickmanParts.clear();
        
        m_StickmanParts.push_back(std::make_pair('O', glm::ivec2( startX,      startY - 30 )));    // head
        m_StickmanParts.push_back(std::make_pair('|', glm::ivec2( startX + 25, startY + 40 )));    // body
        m_StickmanParts.push_back(std::make_pair('/', glm::ivec2( startX - 15, startY + 40 )));    // left arm
        m_StickmanParts.push_back(std::make_pair('\\', glm::ivec2(startX + 35, startY + 40 )));   // right arm
        m_StickmanParts.push_back(std::make_pair('/', glm::ivec2( startX - 15, startY + 100)));    // left leg
        m_StickmanParts.push_back(std::make_pair('\\', glm::ivec2(startX + 35, startY + 100)));   // right leg
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
        SetStickman();
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
                std::cout << "YES WON WOHOOOOO\n";
                break; // Exit early as the word is complete
            }
        }
    }


    void PlayState::RenderHangman()
    {
        for(auto& part : m_PoleParts)
        {
            m_StickmanRenderer->RenderText(part.second.x, part.second.y, COLOR_LIGHTORANGE, std::string(1, part.first));
        }

        switch (m_WrongGuesses)
        {
        case 1:
            m_StickmanRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            break;
        case 2:
            m_StickmanRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            break;
        case 3:
            m_StickmanRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            break;
        case 4:
            m_StickmanRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[3].second.x, m_StickmanParts[3].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[3].first));
            break;
        case 5:
            m_StickmanRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[3].second.x, m_StickmanParts[3].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[3].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[4].second.x, m_StickmanParts[4].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[4].first));
            break;
        case 6:
            m_StickmanRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[3].second.x, m_StickmanParts[3].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[3].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[4].second.x, m_StickmanParts[4].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[4].first));
            m_StickmanRenderer->RenderText(m_StickmanParts[5].second.x, m_StickmanParts[5].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[5].first));
            m_Lost = true;
            break;
        default:
            break;
        }
    }

    void PlayState::RenderCorrectLetters()
    {
        for(auto& letter : m_CorrectLetters)
        {
            auto it = m_LetterToLineMap.find(letter);
            if (it != m_LetterToLineMap.end())
            {
                for(auto& line : it->second)
                {
                    if(letter == 'I')
                        m_WordRenderer->RenderText(line.x+6, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                    else
                        m_WordRenderer->RenderText(line.x, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                }
            }
        }
    }
}