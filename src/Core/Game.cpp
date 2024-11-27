#include "Game.hpp"
#include <iostream>
#include <memory>
#include <exception>
#include <functional> 

#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"
#include "GameStates/MenuState.hpp"
#include "GameStates/PlayState.hpp"
#include "GameStates/SubjectMenuState.hpp"
#include "GameContext.hpp"

namespace isaac_hangman
{
    Game::Game() : m_IsRunning(false)
    {
        m_Background = nullptr;
    }

    bool Game::Init(std::string title, int width, int height)
    {
        GameContext& context = GameContext::GetInstance();
        context.Init(title,width,height);

        if (TTF_Init() == -1) 
        {
            std::cerr << "TTF could not initialize! SDL_ttf Error: " << TTF_GetError() << '\n';
            SDL_Quit();
            return 1;
        }

        if (IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) == 0) 
        {
            std::cerr << "IMG_Init Error: " << IMG_GetError() << std::endl;
            TTF_Quit();
            SDL_Quit();
            return 1;
        }

        m_Background = std::make_unique<Texture>();
        if (!m_Background->CreateTexture(context.GetRenderer().get(), "Assets/bg.jpg"))
        {
            std::cerr << "Failed to load background texture!\n";
        }

        return true;
    }

    void Game::Run()
    {
        if (!Init("Isaac's Hangman", SCREEN_WIDTH, SCREEN_HEIGHT))
        {
            std::cout << "Failed to Init Engine!\n";
            return;
        }

        m_StateManager.SetState(std::make_shared<SubjectMenuState>(m_StateManager));

        m_IsRunning = true;
        while (m_IsRunning)
        {
            HandleEvents();
            Update(0.0f);
            Render();
        }

        Clean();
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
        GameContext& context = GameContext::GetInstance();
        
        SDL_SetRenderDrawColor(context.GetRenderer().get(), 255, 255, 255, 255);
        SDL_RenderClear(context.GetRenderer().get());

        if(m_Background)
            m_Background->Render(context.GetRenderer().get(),0,0);
        
        m_StateManager.Render();

        SDL_RenderPresent(context.GetRenderer().get());
    }

    void Game::Clean()
    {
        GameContext::GetInstance().Clean();
    }
}