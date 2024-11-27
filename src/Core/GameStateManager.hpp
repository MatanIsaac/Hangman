#pragma once
#include <memory>
#include <stack>
#include "GameStates/IGameState.hpp"

namespace isaac_hangman
{
    class GameStateManager
    {
    public:
        // Push a new state onto the stack
        void PushState(std::shared_ptr<IGameState> newState)
        {
            m_StateStack.push(newState);
        }

        // Pop the current state off the stack
        void PopState()
        {
            if (!m_StateStack.empty())
            {
                m_StateStack.pop();
            }
        }

        // Get the current state (top of the stack)
        std::shared_ptr<IGameState> GetCurrentState() const
        {
            if (!m_StateStack.empty())
                return m_StateStack.top();
            return nullptr;
        }

        // Process input for the current state
        void ProcessInput()
        {
            if (!m_StateStack.empty())
                m_StateStack.top()->ProcessInput();
        }

        // Update the current state
        void Update(float deltaTime)
        {
            if (!m_StateStack.empty())
                m_StateStack.top()->Update(deltaTime);
        }

        // Render the current state
        void Render()
        {
            if (!m_StateStack.empty())
                m_StateStack.top()->Render();
        }

    private:
        std::stack<std::shared_ptr<IGameState>> m_StateStack; // Stack of game states
    };
}
