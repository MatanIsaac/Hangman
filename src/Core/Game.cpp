#include "Game.hpp"
#include <iostream>
#include <memory>
#include <exception>
#include <functional> 

#include "Util/ColorMacros.hpp"
#include "GameStates/MenuState.hpp"
#include "GameStates/PlayState.hpp"
#include "GameStates/SubjectMenuState.hpp"
#include "Core/SoundManager.hpp"

namespace isaac_hangman
{
    Game::Game()
        : m_Window(nullptr), m_Renderer(nullptr), m_IsRunning(false), m_Background(nullptr)
    {}

    bool Game::Init(std::string title, int width, int height)
    {
        if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        {
            printf("SDL_Init Error: %s", SDL_GetError());
            return false;
        }

        if (TTF_Init() == -1) 
        {
            std::cerr << "TTF could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
            return false;
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) 
        {
            std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
            return false;
        }

        if(!SoundManager::GetInstance().Init())
        {
            printf("Failed to init SoundMananger!\n");
            TTF_Quit();
            IMG_Quit();
            return false;
        }

        CreateWindowAndRenderer(title,width,height);

        m_Background.reset(new Texture());
    
        if (!m_Background->CreateTexture("Assets/bg.jpg",m_Renderer.get()))
        {
            std::cerr << "Failed to load background texture!\n";
        }

        m_IsRunning = true;
        return true;
    }

    void Game::Run()
    {
        if (!Init("Isaac's Hangman", SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            std::cout << "Failed to Initialize Hangman!\n";
            Clean();
            return;
        }

        m_StateManager.PushState(std::make_shared<MenuState>(m_StateManager,*this));
        
        while (m_IsRunning)
        {
            HandleEvents();
            Update(0.0f);
            Render();
        }
        Clean();
    }

    void Game::Clean()
    {
        if (m_Window)
            m_Window.reset();   

        if (m_Renderer)
            m_Renderer.reset();
                
        m_StateManager.Clean();
        SoundManager::GetInstance().Clean();
        TTF_Quit();
        IMG_Quit();
        SDL_Quit();
    }

    void Game::HandleEvents()
    {
        m_StateManager.ProcessInput();

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                m_IsRunning = false;
                break;
            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_IsRunning = false;
                }
                break;
            }
        }
    }
    void Game::Update(float deltaTime)
    {
        m_StateManager.Update(deltaTime);
    }

    void Game::Render()
    {     
        SDL_SetRenderDrawColor(m_Renderer.get(), 255, 255, 255, 255);
        SDL_RenderClear(m_Renderer.get());

        if(m_Background)
            m_Background->Render(m_Renderer.get() ,0,0);
        
        m_StateManager.Render(m_Renderer.get());

        SDL_RenderPresent(m_Renderer.get());
    }
    
    bool Game::CreateWindowAndRenderer(const std::string &title, int width, int height)
    {
        
        m_Window.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));
        if (m_Window == nullptr)
        {
            SDL_Log("SDL_CreateWindow Error: %s", SDL_GetError());
            SDL_Quit();
            return false;
        }

        m_Renderer.reset(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
        if (m_Renderer == nullptr)
        {
            SDL_Log("SDL_CreateRenderer Error: %s", SDL_GetError());
            SDL_Quit();
            return false;
        }

        return true;
    }
    
}