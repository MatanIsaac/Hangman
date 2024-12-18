#pragma once
#include <memory>
#include <string>
#include <functional>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

#include "Util/Common.hpp"
#include "GameStates/IGameState.hpp"
#include "Graphics/Texture.hpp"
#include "GameStateManager.hpp"
#include "IGame.hpp"

namespace isaac_hangman
{
    class Game : public IGame
    {
    public:
        Game();
        virtual ~Game() {}

        void Run();
        void Clean();

        private:
        bool Init(std::string title, int width, int height);

        void HandleEvents();
        void Update(float deltaTime);
        void Render();
        void Quit() { m_IsRunning = false; }
    private:
        bool CreateWindowAndRenderer(const std::string& title, int width, int height);
    
    private:
        Unique_SDL_Window   m_Window;
        Unique_SDL_Renderer m_Renderer;
        bool                m_IsRunning;
        Unique_Texture      m_Background;
        GameStateManager    m_StateManager;
    };
}