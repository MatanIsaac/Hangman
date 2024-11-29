#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include "../third_party/SDL_FontCache.h" 
#include "glm/glm.hpp"


namespace isaac_hangman
{
    using UniqueTextRenderer = std::unique_ptr<class TextRenderer>;

    class TextRenderer
    {
    public:
        
        // Constructor
        TextRenderer(int fontSize);

        // Destructor
        ~TextRenderer();

        // Copy Constructor
        TextRenderer(const TextRenderer& other);

        // Move Constructor
        TextRenderer(TextRenderer&& other) noexcept;

        // Copy Assignment Operator
        TextRenderer& operator=(const TextRenderer& other);

        // Move Assignment Operator
        TextRenderer& operator=(TextRenderer&& other) noexcept;

        // Clean up resources
        void Clean();
        
        // Render text
        void RenderText(float x, float y, SDL_Color color, const std::string& text);

        // Clone method
        UniqueTextRenderer Clone() const
        {
            return std::make_unique<TextRenderer>(*this);
        }

        // Getters and setters
        const FC_Font& GetFont() const { return *m_Font; }
        const glm::vec2& GetPosition() const { return m_Position; }
        const glm::vec2& GetScale() const { return m_Scale; }
        void SetPosition(const glm::vec2 newPosition) { m_Position = newPosition; }

    private:
        int         m_FontSize;
        glm::vec2   m_Scale; 
        SDL_Rect    m_Rect; 
        FC_Font*    m_Font; 
        glm::vec2   m_Position;
    };

}
