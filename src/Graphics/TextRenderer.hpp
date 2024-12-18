#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <functional>
#include "glm/glm.hpp"
#include "Texture.hpp"
#include "Util/Common.hpp"

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

        // Render text
        void RenderText(SDL_Renderer* renderer,float x, float y, SDL_Color color, const std::string& text);

        // Getters and setters
        glm::vec2 GetTextDimensions(const std::string& text);

    private:
        TextRenderer();
        ~TextRenderer() = default;

        int             m_FontSize;
        Unique_TTF_Font m_Font; 

        Texture m_TextTexture;
    };
}
