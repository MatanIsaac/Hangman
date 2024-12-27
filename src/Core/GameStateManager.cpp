#include "GameStateManager.hpp"

namespace isaac_hangman
{    
    GameStateManager* GameStateManager::s_Instance = nullptr;

    GameStateManager* GameStateManager::GetInstance()
    {
        if(s_Instance == nullptr)
        {
            s_Instance = new GameStateManager();
        }
        return s_Instance;
    }
        

    void GameStateManager::Clean()
    {
        while(m_StateStack.size() > 0)
        {
            m_StateStack.pop();
        }
    }

    void GameStateManager::PushState(std::shared_ptr<IGameState> newState)
    {
        m_StateStack.push(newState);
    }

    // Pop the current state off the stack
    void GameStateManager::PopState()
    {
        if (!m_StateStack.empty())
        {
            m_StateStack.pop();
        }
    }

    // Get the current state (top of the stack)
    std::shared_ptr<IGameState> GameStateManager::GetCurrentState() const
    {
        if (!m_StateStack.empty())
            return m_StateStack.top();
        return nullptr;
    }

    // Process input for the current state
    void GameStateManager::ProcessInput()
    {
        if (!m_StateStack.empty())
            m_StateStack.top()->ProcessInput();
    }

    // Update the current state
    void GameStateManager::Update(float deltaTime)
    {
        if (!m_StateStack.empty())
            m_StateStack.top()->Update(deltaTime);
    }

    // Render the current state
    void GameStateManager::Render(SDL_Renderer* renderer)
    {
        if (!m_StateStack.empty())
            m_StateStack.top()->Render(renderer);
    }

    void GameStateManager::PrintTop()
    {
        if (!m_StateStack.empty())
        {
            auto state = m_StateStack.top();
            if (state != nullptr)
            {
                std::cout << "Stack Top State: " << state->m_StateName << '\n';
            }
            else
            {
                std::cout << "Top state is null.\n";
            }
        }
        else
        {
            std::cout << "Stack is empty.\n";
        }
    }

}


