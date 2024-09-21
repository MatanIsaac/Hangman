#pragma once
#include <memory>
#include <string>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include "GameStates/IGameState.hpp"
#include "Graphics/Texture.hpp"

struct SDLWindowDeleter
{
    void operator()(SDL_Window *w) const { SDL_DestroyWindow(w); }
};

struct SDLRendererDeleter
{
    void operator()(SDL_Renderer *r) const { SDL_DestroyRenderer(r); }
};

enum struct GameStateType
{
    NONE = 0,
    MENU,
    PLAY,
    SUBJECT
};

class Game
{
public:
    Game();
    ~Game();

    void RunGame();
    void QuitGame() { m_IsRunning = false; }

    SDL_Renderer *GetRenderer() { return m_Renderer.get(); }

    void ChangeState(GameStateType state, IGameState* statePtr = nullptr);
private:
    bool Init(std::string title, int width, int height);

    void HandleEvents();
    void Update(float deltaTime);
    void Render();
    void Clean();

    bool m_IsRunning;

    std::unique_ptr<SDL_Window, SDLWindowDeleter> m_Window;
    std::unique_ptr<SDL_Renderer, SDLRendererDeleter> m_Renderer;

    std::unique_ptr<Texture> m_Background;

    IGameState* m_CurrentState;
};
