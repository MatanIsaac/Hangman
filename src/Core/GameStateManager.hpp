#pragma once
#include <memory>
#include <iostream>
#include <stack>
#include "GameStates/IGameState.hpp"

namespace isaac_hangman
{    
    class GameStateManager
    {
    public:
        static GameStateManager* GetInstance();
    
        void Clean();

        void PushState(std::shared_ptr<IGameState> newState);

        // Pop the current state off the stack
        void PopState();

        // Get the current state (top of the stack)
        std::shared_ptr<IGameState> GetCurrentState() const;
        // Process input for the current state
        void ProcessInput();

        // Update the current state
        void Update(float deltaTime);

        // Render the current state
        void Render(SDL_Renderer* renderer);

        void PrintTop();

    private:
        GameStateManager()  = default;
        ~GameStateManager()
        {
            delete s_Instance;
        }
        
        static GameStateManager* s_Instance;
        inline static std::stack<std::shared_ptr<IGameState>> m_StateStack; 
    };

}

