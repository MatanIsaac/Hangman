#include "PlayState.hpp"
#include "Game.hpp"
#include "Util/ColorMacros.hpp"
#include "Util/Common.hpp"

#include <string>
#include <cstring>
#include <algorithm>
#include <iostream>

PlayState::PlayState(Game* game, const std::string& randomWord, Subject::Subjects subject)
    : m_Game(game), m_Word(randomWord), m_CurrentSubject(subject)
{
    std::cout << "Play CTOR\n";
    m_WrongGuesses = 0;
    m_Lose = false;

    FillLetterBank(); 

    m_WordRenderer.reset(new TextRenderer
	(
		game->GetRenderer(),
		"Assets\\fonts\\Filmcryptic.ttf",
		40
	));

    m_PartsRenderer.reset(new TextRenderer
	(
		game->GetRenderer(),
		"Assets\\fonts\\Filmcryptic.ttf",
		90
	));
    
	float xAxisCenter = SCREEN_WIDTH / 2.f;

	glm::vec2 playButtonPosition( glm::vec2( xAxisCenter - 30, ( SCREEN_HEIGHT / 2.f ) ) );
	m_PlayAgainButton.reset(new Button(m_Game->GetRenderer(), "Play", 36, playButtonPosition));

    glm::vec2 quitButtonPosition( glm::vec2( xAxisCenter - 30, ( SCREEN_HEIGHT / 2.f ) + 50.f ) );
	m_QuitButton.reset(new Button(m_Game->GetRenderer(), "Quit", 36, quitButtonPosition)); 

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

    m_StickmanParts.push_back(std::make_pair('O', glm::ivec2( startX,      startY - 30 )));    // head
    m_StickmanParts.push_back(std::make_pair('|', glm::ivec2( startX + 25, startY + 40 )));    // body
    m_StickmanParts.push_back(std::make_pair('/', glm::ivec2( startX - 15, startY + 40 )));    // left arm
    m_StickmanParts.push_back(std::make_pair('\\', glm::ivec2(startX + 35, startY + 40 )));   // right arm
    m_StickmanParts.push_back(std::make_pair('/', glm::ivec2( startX - 15, startY + 100)));    // left leg
    m_StickmanParts.push_back(std::make_pair('\\', glm::ivec2(startX + 35, startY + 100)));   // right leg

}
    
PlayState::~PlayState() 
{
    std::cout << "Play DTOR\n";
    Clean();
}

void PlayState::ProcessInput()
{
    if(m_Lose == false)
    {
        for (Button* button : m_LettersButtons)
        {
            button->ProcessInput();
            if (!button)
            {
                std::cout << "ERROR in PlayState ProcessInput" << '\n';
                return;
            }

            if (button->isButtonPressed() && !button->GetButtonLocked())
            {
                button->SetButtonLock(true);
            }
            else if (!button->isButtonPressed() && button->GetButtonLocked())
            {
                button->SetButtonLock(false);
                std::string letter = button->GetButtonText();
                int letterExists = doesLetterExist(letter[0]);
                if(letterExists == 1)
                { 
                    auto it = m_LetterToLineMap.find(letter[0]);
                    if (it != m_LetterToLineMap.end())
                    {
                        std::cout << "Letter " << letter[0] << " found in the map\n";
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
                        std::cout << "Letter " << letter[0] << " found twice in the map\n";
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
                        std::cout << "Letter " << letter[0] << " found twice in the map\n";
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
                    std::cout << "Letter " << letter[0] << " does not exists in the map\n";
                    
                    m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());

                    // Draw stickman
                    m_WrongGuesses++;
                }
                
            }
        }
    }
    else
    {
        m_PlayAgainButton->ProcessInput();
        if(m_PlayAgainButton->isButtonPressed() && !m_PlayAgainButton->GetButtonLocked())
        {
            m_PlayAgainButton->SetButtonLock(true);
        }
        else if(!m_PlayAgainButton->isButtonPressed() && m_PlayAgainButton->GetButtonLocked())
        {
            m_PlayAgainButton->SetButtonLock(false);
            m_Game->ChangeState(GameStateType::SUBJECT);
        }

        m_QuitButton->ProcessInput();
        if (m_QuitButton)
        {
            if (m_QuitButton->isButtonPressed() && !m_QuitButton->GetButtonLocked())
            {
                m_QuitButton->SetButtonLock(true);
            }
            else if (!m_QuitButton->isButtonPressed() && m_QuitButton->GetButtonLocked())
            {
                m_QuitButton->SetButtonLock(false);
                m_Game->QuitGame();	
            }
        }
    }    
}

void PlayState::Update( float deltaTime )
{
    if(m_Lose == false)
    {
        if(m_WordChars.size() == m_Word.length())
        {
            m_Lose = false;
            m_Word = Subject::GetRandomWord(m_CurrentSubject);
            m_WordChars.clear();
            m_LettersButtons.clear();
            FillLetterBank();
            m_WrongGuesses = 0;
        }
    }
}

void PlayState::Render( SDL_Renderer* renderer )
{
    if(m_Lose == false)
    {
        auto subject = Subject::SubjectToString(m_CurrentSubject);
        auto str = "Subject: " + *subject;
        m_WordRenderer->RenderText(25, 25, COLOR_LIGHTORANGE, str);

        for (Button* button : m_LettersButtons)
        {
            button->Render();
        }

        RenderLinePerLetter(renderer);

        for(auto& letter : m_WordChars)
        {
            auto it = m_LetterToLineMap.find(letter);
            if (it != m_LetterToLineMap.end())
            {
                for(auto& line : it->second)
                {
                    m_WordRenderer->RenderText(line.x, line.y - 80, COLOR_LIGHTORANGE, std::string(1, letter));
                }
            }
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
    if(m_Lose == true)
    {
        m_WordRenderer->RenderText(SCREEN_WIDTH / 2 - 65, SCREEN_HEIGHT / 5, COLOR_LIGHTORANGE, "You Lose!");
        m_WordRenderer->RenderText(SCREEN_WIDTH / 5, SCREEN_HEIGHT / 5 + 100, COLOR_LIGHTORANGE, "Would You Like To Play Again?");
        m_PlayAgainButton->Render();
        m_QuitButton->Render();
    }
}

void PlayState::Clean()
{
	delete m_Game;
}


void PlayState::FillLetterBank()
{
    int buttonWidth = 30;
    float x = 20.f;
    float y = SCREEN_HEIGHT - 100.f;
    float margin = 0.f;
    int cnt = 0;
    
    for (char l = 'A'; l <= 'Z'; ++l) 
    {
        std::string s(1, l);
        const char* c = s.c_str();
        Button* button = new Button(m_Game->GetRenderer(), c, 25, glm::vec2(0, 0));
        m_LettersButtons.push_back(button);
        if(x + buttonWidth > SCREEN_WIDTH)
        {
            x = 200.f;
            y += buttonWidth + margin;
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
    std::cout << "Letter " << letter << " exists " << cnt << " times in the word\n";
    return cnt;
}

void PlayState::RenderLinePerLetter(SDL_Renderer* renderer)
{
    // Render lines for each letter in m_Word
    int wordLength = m_Word.length();             
    int startX = SCREEN_WIDTH / 2;
    int startY = SCREEN_HEIGHT / 2;
    int margin = 30;
    for (int i = 0; i < wordLength; i++)
    {
        auto it = m_LetterToLineMap.find(m_Word[i]);
        if (it == m_LetterToLineMap.end())
            m_LetterToLineMap[m_Word[i]] = std::vector<glm::ivec2>{glm::ivec2(startX + 5, startY)};
        else
            it->second.push_back(glm::ivec2(startX + 5, startY));
        
        m_WordRenderer->RenderText(startX + 5, startY - 60, COLOR_LIGHTORANGE, "_");
        startX += margin;
    }
}