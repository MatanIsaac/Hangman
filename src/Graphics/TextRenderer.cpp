#include "TextRenderer.hpp"
#include "Core/GameContext.hpp"

namespace isaac_hangman
{
    glm::vec2 TextRenderer::GetTextDimensions(const std::string& text)
    {
        if (m_Font)
        {
            int width = FC_GetWidth(m_Font, "%s", text.c_str());
            int height = FC_GetHeight(m_Font, "%s", text.c_str());
            return glm::vec2(width, height);
        }
        return glm::vec2(0.f, 0.f);
    }
    
    TextRenderer::TextRenderer()
        :
        m_FontSize(35),
        m_Rect({ 0, 0, 0, 0 }),
        m_Font(FC_CreateFont()),
        m_Position(0.f,0.f)
    {
        GameContext& context = GameContext::GetInstance();

        if (FC_LoadFont(m_Font, context.GetRenderer().get(), "Assets/fonts/Filmcryptic.ttf", 
            m_FontSize, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL) == 0)
        {
            SDL_Log("Failed to load font: %s", SDL_GetError());
        }
    }

    TextRenderer::~TextRenderer()
    {
        Clean();
    } 

    void TextRenderer::Clean()
    {
        if (m_Font)
        {
            FC_FreeFont(m_Font);
            m_Font = nullptr;
        }
    }
       
    void TextRenderer::RenderText(float x, float y, SDL_Color color, const std::string& text)
    {
        GameContext& context = GameContext::GetInstance();

        if (m_Font && context.GetRenderer())
        {
            m_Rect = FC_DrawColor(m_Font, context.GetRenderer().get(), x, y, color, text.c_str());
        }
        else
        {
            SDL_Log("Failed to render text at text renderer");
        }
    }   
};
