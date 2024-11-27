#pragma once

#include <SDL.h>
#include <string>
#include <memory>
#include <functional>
#include <stdexcept>

namespace isaac_hangman
{
    using UniqueRenderer = std::unique_ptr<SDL_Renderer,std::function<void(SDL_Renderer *)>>;
    using UniqueWindow = std::unique_ptr<SDL_Window, std::function<void(SDL_Window *)>>;

    class GameContext
    {
    public:
        // Delete copy and move constructors to enforce singleton
        GameContext(const GameContext&) = delete;
        GameContext& operator=(const GameContext&) = delete;
        GameContext(GameContext&&) = delete;
        GameContext& operator=(GameContext&&) = delete;

        UniqueWindow m_Window;
        UniqueRenderer m_Renderer;
        bool m_IsRunning = false;

        // Access the single instance of GameContext
        static GameContext& GetInstance()
        {
            static GameContext instance; // Lazy-initialized singleton
            return instance;
        }

        // Initialize SDL and create window/renderer
        bool Init(const std::string& title, int width, int height)
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
                return false;
            }

            m_Window = UniqueWindow
            (
                SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN),
                SDL_DestroyWindow
            );
            if (m_Window == nullptr)
            {
                std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
                SDL_Quit();
                return false;
            }

            m_Renderer = UniqueRenderer
            (
                SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
                SDL_DestroyRenderer
            );
            if (m_Renderer == nullptr)
            {
                std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
                SDL_DestroyWindow(m_Window.get());
                SDL_Quit();
                return false;
            }

            return true;
        }

        // Cleanup SDL resources
        void Clean()
        {
            if (m_Renderer)
            {
                SDL_DestroyRenderer(m_Renderer.get());
                m_Renderer = nullptr;
            }

            if (m_Window)
            {
                SDL_DestroyWindow(m_Window.get());
                m_Window = nullptr;
            }

            SDL_Quit();
        }

        // Access SDL_Renderer
        const UniqueRenderer& GetRenderer() const
        {
            return m_Renderer;
        }

        // Access SDL_Window
        const UniqueWindow& GetWindow() const
        {
            return m_Window;
        }

    private:
        GameContext() : m_Window(nullptr), m_Renderer(nullptr) {}

        ~GameContext()
        {
            Clean();
        }
    };
}