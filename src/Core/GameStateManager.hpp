#pragma once
#include <memory>
#include "GameStates/IGameState.hpp"

namespace isaac_hangman
{
    class GameStateManager
    {
    public:
        void SetState(std::shared_ptr<IGameState> newState)
        {
            m_CurrentState = newState;
        }

        void ProcessInput()
        {
            if (m_CurrentState)
                m_CurrentState->ProcessInput();
        }

        void Update(float deltaTime)
        {
            if (m_CurrentState)
                m_CurrentState->Update(deltaTime);
        }

        void Render()
        {
            if (m_CurrentState)
                m_CurrentState->Render();
        }

    private:
        std::shared_ptr<IGameState> m_CurrentState;
    };
}
