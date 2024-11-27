#pragma once
#include <memory>
#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "GameStates/IGameState.hpp"
#include "Graphics/Texture.hpp"
#include "GameStateManager.hpp"

namespace isaac_hangman
{
    class Game
    {
    public:
        Game();
        ~Game() = default;

        void Run();
        void Quit() { m_IsRunning = false; }

        private:
        bool Init(std::string title, int width, int height);

        void HandleEvents();
        void Update(float deltaTime);
        void Render();
        void Clean();

    private:
        GameStateManager m_StateManager;
        bool m_IsRunning;

        std::unique_ptr<Texture> m_Background;
    };
}