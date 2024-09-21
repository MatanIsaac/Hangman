#include "Game.hpp"
#include <iostream>
#include <memory>
#include <exception>

#include "Util/Common.hpp"
#include "Util/ColorMacros.hpp"
#include "GameStates/MenuState.hpp"
#include "GameStates/PlayState.hpp"
#include "GameStates/SubjectMenuState.hpp"

Game::Game()
    : m_IsRunning(false), m_Window(nullptr), m_Renderer(nullptr), m_CurrentState(nullptr)
{
    std::cout << "Game CTOR\n";
}

Game::~Game()
{
    std::cout << "Game DTOR\n";
}

bool Game::Init(std::string title, int width, int height)
{
    std::cout << "Menu Init\n";
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << '\n';
        return false;
    }

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

    m_Window.reset(SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN));
    if (m_Window == nullptr)
    {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return false;
    }

    m_Renderer.reset(SDL_CreateRenderer(m_Window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));
    if (m_Renderer == nullptr)
    {
        std::cerr << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(m_Window.get());
        SDL_Quit();
        return false;
    }
   

    ChangeState(GameStateType::MENU);

    return true;
}

void Game::RunGame()
{
    if (!Init("Isaac's Hangman", SCREEN_WIDTH, SCREEN_HEIGHT))
    {
        std::cout << "Failed to Init Engine!\n";
        return;
    }

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
    m_CurrentState->ProcessInput();

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
    m_CurrentState->Update(0.0f);

}

void Game::Render()
{
    SDL_SetRenderDrawColor(m_Renderer.get(), 255, 255, 255, 255);
    SDL_RenderClear(m_Renderer.get());

    m_CurrentState->Render(m_Renderer.get());
    SDL_RenderPresent(m_Renderer.get());
}

void Game::Clean()
{
    delete m_CurrentState;

    SDL_Quit();
}

void Game::ChangeState(GameStateType state, IGameState* statePtr)
{
    try
    {
        if(statePtr)
        {
            m_CurrentState = statePtr;
            return;
        }
        else
        {
            switch (state)
            {
            case GameStateType::MENU:
                m_CurrentState = new MenuState(this);
                break;
            case GameStateType::PLAY:
                m_CurrentState = new PlayState(this,"");
                break;
            case GameStateType::SUBJECT:
                m_CurrentState = new SubjectMenuState(this);
                break;
            default:
                std::cerr << "Unknown state!" << std::endl;
                return;
            }

            if (!m_CurrentState)
            {
                std::cerr << "State change failed, current state is null!" << std::endl;
            }
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception during state change: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "Unknown exception during state change" << std::endl;
    }
}