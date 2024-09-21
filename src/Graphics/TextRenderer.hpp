#pragma once

#include <SDL.h>
#include <string>
#include <iostream>
#include "../third_party/SDL_FontCache.h" 
#include "glm/glm.hpp"

class TextRenderer
{
public:
    TextRenderer(SDL_Renderer* renderer, std::string fontPath, int fontSize, SDL_Color fontColor)
        : m_Renderer(renderer)
    {
        m_Font = FC_CreateFont();
        if (FC_LoadFont(m_Font, renderer, fontPath.c_str(), 
            fontSize, FC_MakeColor(fontColor.r, fontColor.g, fontColor.b, fontColor.a), TTF_STYLE_NORMAL) == 0)
        {
            SDL_Log("Failed to load font: %s", SDL_GetError());
        }
        
        m_Rect = { 0, 0, 0, 0 };
    }
    ~TextRenderer()
    {
        Clean();
    }

    void Clean()
    {
        if (m_Font)
        {
            FC_FreeFont(m_Font);
        }
    }

    void RenderText(float x, float y, SDL_Color color, const std::string& text)
    {
        if (m_Font && m_Renderer)
        {
            m_Rect = FC_DrawColor(m_Font, m_Renderer, x, y, color,text.c_str());
            m_Scale = { m_Rect.w, m_Rect.h };
        }
        else
        {
            SDL_Log("Failed to render text: Font not loaded.");
        }
    }

    const FC_Font& GetFont() const { return *m_Font; }
    const glm::vec2& GetPosition() const { return m_Position; }
    const glm::vec2& GetScale() const { return m_Scale; }

    void SetPosition(const glm::vec2& position) { m_Position = position; }
    void SetScale(const glm::vec2& scale) { m_Scale = scale; }

private:
    FC_Font* m_Font;
    SDL_Rect m_Rect;
    glm::vec2 m_Position;
    glm::vec2 m_Scale;
    SDL_Renderer* m_Renderer;
};

struct TextRendererDeleter
{
	void operator()(TextRenderer *t) const
	{
		t->Clean();
		delete t;
	}
};
