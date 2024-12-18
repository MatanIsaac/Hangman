#include "TextRenderer.hpp"
#include "Core/Game.hpp"

namespace isaac_hangman
{
    glm::vec2 TextRenderer::GetTextDimensions(const std::string& text)
    {
        if (m_Font)
        {
            int width;
            int height;
            TTF_SizeText(m_Font.get(),text.c_str(),&width,&height);
            return glm::vec2(width, height);
        }
        return glm::vec2(0.f, 0.f);
    }
    
    TextRenderer::TextRenderer()
        : m_FontSize(35)
    {
        //m_TextTexture.reset(new Texture());
        m_Font.reset(TTF_OpenFont("Assets/fonts/Filmcryptic.ttf",m_FontSize));
        if (m_Font == nullptr)
        {
            SDL_Log("Failed to load font: %s", TTF_GetError());
            return;
        }
    }
       
    void TextRenderer::RenderText(SDL_Renderer* renderer,float x, float y, SDL_Color color, const std::string& text)
    {
        if (m_Font) 
        {
            m_TextTexture.CreateTextureFromText(m_Font.get(),text,color,renderer);
            m_TextTexture.Render(renderer,x,y);
        }
    }
};
