#include "PlayState.hpp"
#include "Game.hpp"
#include "Util/ColorMacros.hpp"
#include "Util/Common.hpp"

#include <string>
#include <algorithm>
#include <iostream>

PlayState::PlayState(Game* game, const std::string& randomWord)
    : m_Game(game)
{
    std::cout << "Play CTOR\n";

    FillLetterBank(); 

    /*
    m_PoleParts.reserve(6);
    int startX = 100;
    int startY = 75;

    auto pole0  = new TextRenderer("_", 100, COLOR_LIGHTORANGE, glm::vec2(startX-20, startY+190));
    auto pole1  = new TextRenderer("_", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY+190));
    auto pole2  = new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY+200));
    auto pole3  = new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY+160));
    auto pole4  = new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY+120));
    auto pole5  = new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY+80));
    auto pole6  = new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY+40));
    auto pole7  = new TextRenderer("_", 100, COLOR_LIGHTORANGE, glm::vec2(startX+20, startY-40));
    auto pole8  = new TextRenderer("_", 100, COLOR_LIGHTORANGE, glm::vec2(startX+60, startY-40));
    auto pole9  = new TextRenderer("_", 100, COLOR_LIGHTORANGE, glm::vec2(startX+100, startY-40));
    auto pole10 = new TextRenderer("_", 100, COLOR_LIGHTORANGE, glm::vec2(startX+140, startY-40));
    auto pole11 = new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX+180, startY+40));

    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole0));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole1));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole2));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole3));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole4));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole5));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole6));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole7));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole8));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole9));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole10));
    m_PoleParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(pole11));
    
    for (auto& part : m_PoleParts)
    {
        part->CreateText(m_Game->GetRenderer());
    }
    
    // Create TextRenderers for each body part of the stickman
    m_StickmanParts.reserve(6);
    startX += 150;
    startY += 125;
    
    auto head       =   new TextRenderer("O", 100, COLOR_LIGHTORANGE, glm::vec2(startX, startY - 30));
    auto body       =   new TextRenderer("|", 100, COLOR_LIGHTORANGE, glm::vec2(startX + 25, startY + 50));
    auto leftArm    =   new TextRenderer("/", 100, COLOR_LIGHTORANGE, glm::vec2(startX - 15, startY + 50));
    auto rightArm   =   new TextRenderer("\\", 100, COLOR_LIGHTORANGE, glm::vec2(startX + 35, startY + 50));
    auto leftLeg    =   new TextRenderer("/", 100, COLOR_LIGHTORANGE, glm::vec2(startX - 15, startY + 110));
    auto rightLeg   =   new TextRenderer("\\", 100, COLOR_LIGHTORANGE, glm::vec2(startX + 35, startY + 110));

    // Initialize TextRenderers for stickman parts using unique_ptr
    m_StickmanParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(head));
    m_StickmanParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(body));
    m_StickmanParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(leftArm));
    m_StickmanParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(rightArm));
    m_StickmanParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(leftLeg));
    m_StickmanParts.push_back(std::unique_ptr<TextRenderer, TextRendererDeleter>(rightLeg));

    for (auto& part : m_StickmanParts)
    {
        part->CreateText(m_Game->GetRenderer());
    }
    */
}
    
PlayState::~PlayState() 
{
    std::cout << "Play DTOR\n";
    Clean();
}

void PlayState::ProcessInput()
{
    
}

void PlayState::Update( float deltaTime )
{

}

void PlayState::Render( SDL_Renderer* renderer )
{
    
}

void PlayState::Clean()
{
	delete m_Game;
}


void PlayState::FillLetterBank()
{

}

bool PlayState::doesLetterExist(char letter) const
{
    return false;
}

void PlayState::RenderLinePerLetter(SDL_Renderer* renderer)
{

}