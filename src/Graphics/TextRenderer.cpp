#include "TextRenderer.hpp"
#include "Core/GameContext.hpp"

namespace isaac_hangman
{    
    TextRenderer::TextRenderer(int fontSize)
        :
        m_FontSize(fontSize),
        m_Scale(0.f,0.f),
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

    TextRenderer::TextRenderer(const TextRenderer& other) 
        : 
        m_FontSize(other.m_FontSize), 
        m_Scale(other.m_Scale), 
        m_Rect(other.m_Rect), 
        m_Font(nullptr), // Initialize before loading
        m_Position(other.m_Position)
    {
        GameContext& context = GameContext::GetInstance();    
        m_Font = FC_CreateFont();
        if (FC_LoadFont(m_Font, context.GetRenderer().get(), "Assets/fonts/Filmcryptic.ttf", 
                        other.m_FontSize, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL) == 0)
        {
            SDL_Log("Failed to load font: %s", SDL_GetError());
        }
    }  
   
    TextRenderer::TextRenderer(TextRenderer&& other) noexcept
        : 
        m_FontSize(other.m_FontSize),
        m_Scale(other.m_Scale), 
        m_Rect(other.m_Rect), 
        m_Font(other.m_Font), 
        m_Position(other.m_Position) 
    {
        other.m_Font = nullptr; // Nullify the moved-from object to avoid double-free
    }   
    
    TextRenderer& TextRenderer::operator=(const TextRenderer& other)
    {
        if (this != &other) // Self-assignment check
        {
            Clean(); // Free existing resources 
            m_FontSize = other.m_FontSize;
            m_Position = other.m_Position;
            m_Scale = other.m_Scale;
            m_Rect = other.m_Rect;  
            GameContext& context = GameContext::GetInstance();  
            m_Font = FC_CreateFont();
            if (FC_LoadFont(m_Font, context.GetRenderer().get(), "Assets/fonts/Filmcryptic.ttf", 
                other.m_FontSize, FC_MakeColor(255, 255, 255, 255), TTF_STYLE_NORMAL) == 0)
            {
                SDL_Log("Failed to load font: %s", SDL_GetError());
            }
        }
        return *this;
    }   
    
    TextRenderer& TextRenderer::operator=(TextRenderer&& other) noexcept
    {
        if (this != &other) // Self-assignment check
        {
            Clean(); // Free existing resources 
            m_Font = other.m_Font;
            m_Rect = other.m_Rect;
            m_Position = other.m_Position;
            m_Scale = other.m_Scale;
            m_FontSize = other.m_FontSize;  
            other.m_Font = nullptr; // Nullify the moved-from object to avoid double-free
        }
        return *this;
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
            m_Scale = { m_Rect.w, m_Rect.h };
        }
        else
        {
            SDL_Log("Failed to render text: %s", SDL_GetError());
        }
    }   
};
