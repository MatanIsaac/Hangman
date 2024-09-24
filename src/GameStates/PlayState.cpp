#include "PlayState.hpp"
#include "Game.hpp"
#include "Util/ColorMacros.hpp"
#include "Util/Common.hpp"

#include <string>
#include <algorithm>
#include <iostream>

PlayState::PlayState(Game* game, const std::string& randomWord)
    : m_Game(game), m_Word(randomWord)
{
    std::cout << "Play CTOR\n";
    wrongGuesses = 0;
    Lose = false;

    FillLetterBank(); 

    m_WordRenderer.reset(new TextRenderer
	(
		game->GetRenderer(),
		"src\\Assets\\fonts\\Filmcryptic.ttf",
		40,
		SDL_Color{255, 183, 77, 255}
	));

    m_PartsRenderer.reset(new TextRenderer
	(
		game->GetRenderer(),
		"src\\Assets\\fonts\\Filmcryptic.ttf",
		90,
		SDL_Color{255, 183, 77, 255}
	));
    
    int startX = 50;
    int startY = 120;
  
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

    startX = 250;
    startY = 200;

    m_StickmanParts.push_back(std::make_pair('O', glm::ivec2( startX,      startY - 30 )));    // head
    m_StickmanParts.push_back(std::make_pair('|', glm::ivec2( startX + 25, startY + 50 )));    // body
    m_StickmanParts.push_back(std::make_pair('/', glm::ivec2( startX - 15, startY + 50 )));    // left arm
    m_StickmanParts.push_back(std::make_pair('\\', glm::ivec2(startX + 35, startY + 50 )));   // right arm
    m_StickmanParts.push_back(std::make_pair('/', glm::ivec2( startX - 15, startY + 110)));    // left leg
    m_StickmanParts.push_back(std::make_pair('\\', glm::ivec2(startX + 35, startY + 110)));   // right leg

}
    
PlayState::~PlayState() 
{
    std::cout << "Play DTOR\n";
    Clean();
}

void PlayState::ProcessInput()
{
    if(!Lose)
    {
        for (Button* button : m_LettersButtons)
        {
            button->ProcessInput();
            if (!button)
            {
                std::cout << "ERROR in PlayState-Button ProcessInput" << '\n';
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
               
                if(doesLetterExist(letter[0]))
                { 
                    auto it = m_LetterToLineMap.find(letter[0]);
                    if (it != m_LetterToLineMap.end())
                    {
                        std::cout << "Letter " << letter[0] << " found in the map\n";
                        m_WordChars.push_back(letter[0]);
                        m_LetterToLineMap.erase(it);
                    }
                    m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());
                }
                else
                {
                    std::cout << "Letter " << letter[0] << " does not exists in the map\n";
                    
                    m_LettersButtons.erase(std::remove_if(m_LettersButtons.begin(), m_LettersButtons.end(), [&](Button* b) { return b->GetButtonText()[0] == letter[0]; }), m_LettersButtons.end());

                    // Draw stickman
                    wrongGuesses++;
                }
                
            }
        }
    }    
}

void PlayState::Update( float deltaTime )
{
    if(Lose)
    {
        std::cout << "You Lose\n";
        m_Game->ChangeState(GameStateType::MENU);
    }
    
}

void PlayState::Render( SDL_Renderer* renderer )
{
    if(!Lose)
    {
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
                    m_WordRenderer->RenderText(line.x + 5, line.y - 30, SDL_Color{255, 183, 77, 255}, std::string(1, letter));
                }
            }
        }

        for(auto& part : m_PoleParts)
        {
            m_PartsRenderer->RenderText(part.second.x, part.second.y, SDL_Color{255, 183, 77, 255}, std::string(1, part.first));
        }
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
        Button* button = new Button(m_Game->GetRenderer(), c, buttonWidth, glm::vec2(0, 0));
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

bool PlayState::doesLetterExist(char letter) const
{
    std::string word = m_Word;
    return word.find(letter) != std::string::npos;
}

void PlayState::RenderLinePerLetter(SDL_Renderer* renderer)
{
    // Render lines for each letter in m_Word
    int wordLength = m_Word.length();
    //int lineLength = 30;              
    int startX = SCREEN_WIDTH / 2;
    int startY = SCREEN_HEIGHT / 2;
    int margin = 50;
    for (int i = 0; i < wordLength; i++)
    {
        //int endX = startX + lineLength;
        //int endY = startY;

        auto it = m_LetterToLineMap.find(m_Word[i]);
        if (it == m_LetterToLineMap.end())
            m_LetterToLineMap[m_Word[i]] = std::vector<glm::ivec2>{glm::ivec2(startX + 5, startY)};
        else
            it->second.push_back(glm::ivec2(startX + 5, startY));
        
        //SDL_SetRenderDrawColor(renderer, COLOR_LIGHTORANGE.r, COLOR_LIGHTORANGE.g, COLOR_LIGHTORANGE.b, 255);
        //SDL_RenderDrawLine(renderer, startX, startY, endX, endY);
        m_PartsRenderer->RenderText(startX + 5, startY - 60, SDL_Color{255, 183, 77, 255}, "_");
        startX += margin;

    }
}