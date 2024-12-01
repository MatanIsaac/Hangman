#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include "../third_party/SDL_FontCache.h" 
#include "glm/glm.hpp"


namespace isaac_hangman
{
    /**
     * TextRenderer is used alot in this project, so in order to save time, 
     * it would be more efficient to make it a singleton, and use it globally in this case.
     */
    class TextRenderer
    {
    public:
        static TextRenderer& GetInstance()
        {
            static TextRenderer instance; 
            return instance;
        }

        // Deleted functions to prevent copying or moving
        TextRenderer(const TextRenderer&) = delete;
        TextRenderer(TextRenderer&&) = delete;
        TextRenderer& operator=(const TextRenderer&) = delete;
        TextRenderer& operator=(TextRenderer&&) = delete;

        // Clean up resources
        void Clean();
        
        // Render text
        void RenderText(float x, float y, SDL_Color color, const std::string& text);

        // Getters and setters
        glm::vec2 GetTextDimensions(const std::string& text);
        const FC_Font& GetFont() const { return *m_Font; }
        const glm::vec2& GetPosition() const { return m_Position; }
        const SDL_Rect& GetRect() const { return m_Rect; }
        void SetPosition(const glm::vec2& newPosition) { m_Position = newPosition; }
    private:
        TextRenderer();
        ~TextRenderer();   

        int         m_FontSize;
        SDL_Rect    m_Rect; 
        FC_Font*    m_Font; 
        glm::vec2   m_Position;
    };
}
