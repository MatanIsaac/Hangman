#pragma once

#include <SDL.h>

namespace isaac_hangman
{
    class IGameState
    {
    public:
        virtual ~IGameState() = default;
        virtual void ProcessInput() = 0;
        virtual void Update(float deltaTime) = 0;
        virtual void Render(SDL_Renderer* renderer) = 0;
        const char* m_StateName = "DefaultStateName";
    };

}