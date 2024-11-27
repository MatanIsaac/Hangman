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
        
    PlayState::PlayState(GameStateManager& stateManager,const std::string& randomWord, Subject::Subjects subject)
        : m_GameStateManager(stateManager),m_Word(randomWord), m_CurrentSubject(subject)
    {
        m_WrongGuesses = 0;
        m_Lose = false;
        m_SpaceRemoved = false;

        m_Timer.Start();
        FillLetterBank(); 
        auto time = m_Timer.Stop();
        std::cout << "Took: " << time << " Seconds\n";

        m_WordRenderer = std::make_unique<TextRenderer>(35);

        m_PartsRenderer = std::make_unique<TextRenderer>(90);
        
        float xAxisCenter = SCREEN_WIDTH / 2.f;

        glm::vec2 nextWordButtonPosition( glm::vec2( xAxisCenter + 40.f, ( SCREEN_HEIGHT / 3.5f ) ) );
        m_NextWordButton = std::make_unique<Button>("Next Word",36,nextWordButtonPosition);

        glm::vec2 BackButtonPosition( glm::vec2( xAxisCenter, ( SCREEN_HEIGHT / 2.5f ) )  );
        m_BackToSubjectsButton.reset(new Button("Back To Subjects", 36, BackButtonPosition));

        glm::vec2 quitButtonPosition( glm::vec2( 35.f, SCREEN_HEIGHT - 85.f ) );
        m_QuitButton.reset(new Button("Quit", 36, quitButtonPosition)); 

        SetStickman();

    }

    void PlayState::ProcessInput()
    {
        if (m_QuitButton->isPressed())
        {
            GameContext::GetInstance().m_IsRunning = false;	
        }

        if(m_Lose == false)
        {
            if(!m_SpaceRemoved && doesLetterExist(' ') == 1)
            { 
                m_WordChars.push_back(' ');
                m_LetterToLineMap.erase(' ');
                m_SpaceRemoved = true;
            }
            
            for (Button* button : m_LettersButtons)
            {
                if(button == nullptr)
                    break;

                if (button->isPressed())
                {
                    std::string letter = button->GetButtonText();
                    int letterExists = 0;doesLetterExist(letter[0]);
                    
                    if(letterExists == 1)
                    { 
                        auto it = m_LetterToLineMap.find(letter[0]);
                        if (it != m_LetterToLineMap.end())
                        {
                            m_WordChars.push_back(letter[0]);
                            m_LetterToLineMap.erase(it);
                        }
                        m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), 
                            [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());
                    }
                    else if(letterExists == 2)
                    {
                        auto it = m_LetterToLineMap.find(letter[0]);
                        if (it != m_LetterToLineMap.end())
                        {
                            m_WordChars.push_back(letter[0]);
                            m_WordChars.push_back(letter[0]);
                            m_LetterToLineMap.erase(it);
                            auto it2 = m_LetterToLineMap.find(letter[0]);
                            if (it2 != m_LetterToLineMap.end()) m_LetterToLineMap.erase(it2);
                        }
                        m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), 
                            [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());
                    }
                    else if(letterExists == 3)
                    {
                        auto it = m_LetterToLineMap.find(letter[0]);
                        if (it != m_LetterToLineMap.end())
                        {
                            m_WordChars.push_back(letter[0]);
                            m_WordChars.push_back(letter[0]); 
                            m_WordChars.push_back(letter[0]); 
                            m_LetterToLineMap.erase(it);
                            auto it2 = m_LetterToLineMap.find(letter[0]);
                            if (it2 != m_LetterToLineMap.end()) m_LetterToLineMap.erase(it2);
                            auto it3 = m_LetterToLineMap.find(letter[0]);
                            if (it3 != m_LetterToLineMap.end()) m_LetterToLineMap.erase(it3);
                        }
                        m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), 
                            [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());
                    }
                    
                    if(letterExists == 0)
                    {
                        m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), 
                            [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());
                        m_WrongGuesses++;
                    }
                }
            }
        }
        else
        {
            if(m_BackToSubjectsButton->isPressed())
            {
                m_GameStateManager.PushState(std::make_shared<SubjectMenuState>(m_GameStateManager));
            }

            if(m_NextWordButton->isPressed())
            {
                // Get Next Word
                m_Word = Subject::GetRandomWord(m_CurrentSubject);
                m_Lose = false;
                m_SpaceRemoved = false;
                
                SetStickman();
            }


        }    
        
    }

    void PlayState::Update( float deltaTime )
    {
        m_QuitButton->Update(deltaTime);
        if(m_Lose == false)
        {
            if(m_WordChars.size() == m_Word.length())
            {
                m_Lose = false;
                m_SpaceRemoved = false;
                m_Word = Subject::GetRandomWord(m_CurrentSubject);
                m_WordChars.clear();
                m_LettersButtons.clear();
                FillLetterBank();
                m_WrongGuesses = 0;
            }
            
            for (auto button : m_LettersButtons)
            {
                button->Update(deltaTime);
            }

        }
        else 
        {   
            m_BackToSubjectsButton->Update(deltaTime);
            m_NextWordButton->Update(deltaTime);
        }
    }

    void PlayState::Render()
    {
        auto subject = Subject::SubjectToString(m_CurrentSubject);
        auto str = "Subject: " + *subject;
        m_WordRenderer->RenderText(25, 25, COLOR_LIGHTORANGE, str);
        m_QuitButton->Render();
        if(m_Lose == false)
        {
            RenderLinePerLetter();

            for(auto& letter : m_WordChars)
            {
                auto it = m_LetterToLineMap.find(letter);
                if (it != m_LetterToLineMap.end())
                {
                    for(auto& line : it->second)
                    {
                        if(letter == 'I')
                            m_WordRenderer->RenderText(line.x+20, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                        else
                            m_WordRenderer->RenderText(line.x, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                    }
                }
            }
            
            for (Button* button : m_LettersButtons)
            {
                button->Render();
            }
        }
        else
        {
            m_WordRenderer->RenderText(SCREEN_WIDTH / 2 + 50, 50,COLOR_LIGHTORANGE, "You Lost!");
            m_WordRenderer->RenderText(SCREEN_WIDTH / 2 - 100, 100,COLOR_LIGHTORANGE, "Would You Like To Play Again?");
            m_BackToSubjectsButton->Render();
            m_NextWordButton->Render();
        }

        for(auto& part : m_PoleParts)
        {
            m_PartsRenderer->RenderText(part.second.x, part.second.y, COLOR_LIGHTORANGE, std::string(1, part.first));
        }

        switch (m_WrongGuesses)
        {
        case 1:
            m_PartsRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            break;
        case 2:
            m_PartsRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_PartsRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            break;
        case 3:
            m_PartsRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_PartsRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_PartsRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            break;
        case 4:
            m_PartsRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_PartsRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_PartsRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            m_PartsRenderer->RenderText(m_StickmanParts[3].second.x, m_StickmanParts[3].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[3].first));
            break;
        case 5:
            m_PartsRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_PartsRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_PartsRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            m_PartsRenderer->RenderText(m_StickmanParts[3].second.x, m_StickmanParts[3].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[3].first));
            m_PartsRenderer->RenderText(m_StickmanParts[4].second.x, m_StickmanParts[4].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[4].first));
            break;
        case 6:
            m_PartsRenderer->RenderText(m_StickmanParts[0].second.x, m_StickmanParts[0].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[0].first));
            m_PartsRenderer->RenderText(m_StickmanParts[1].second.x, m_StickmanParts[1].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[1].first));
            m_PartsRenderer->RenderText(m_StickmanParts[2].second.x, m_StickmanParts[2].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[2].first));
            m_PartsRenderer->RenderText(m_StickmanParts[3].second.x, m_StickmanParts[3].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[3].first));
            m_PartsRenderer->RenderText(m_StickmanParts[4].second.x, m_StickmanParts[4].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[4].first));
            m_PartsRenderer->RenderText(m_StickmanParts[5].second.x, m_StickmanParts[5].second.y, COLOR_LIGHTORANGE, std::string(1, m_StickmanParts[5].first));
            m_Lose = true;
            break;
        default:
            break;
        }
        
    }

    // OPTIMIZE: Creating 26 button and emplacing them / inserting into an unordered map 
    // takes about 0.25 seconds, slow.  
    void PlayState::FillLetterBank()
    {
        int buttonWidth = 30;
        float x = 200.f;
        float y = SCREEN_HEIGHT - 200.f;
        float margin = 0.f;
        int cnt = 0;
        
        for (char l = 'A'; l <= 'Z'; ++l) 
        {
            auto btn = new Button(&l, 40, glm::vec2(0, 0));
            m_LettersButtons.emplace_back(btn);
            
            if(cnt == 13)
            {
                x = 200.f;
                y += buttonWidth + 20.f;
            }
            m_LettersButtons.at(cnt)->SetPosition(glm::vec2(x, y));
            x += margin + buttonWidth;
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

    void PlayState::RenderLinePerLetter()
    {
        // Render lines for each letter in m_Word
        int wordLength = m_Word.length();             
        int startX = SCREEN_WIDTH / 3 + 50;
        int startY = SCREEN_HEIGHT / 2 - 100;
        int margin = 30;
        int spaceCnt = 0;
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
}