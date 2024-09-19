#pragma once

#include <SDL.h>

class IGameState
{
public:
    virtual ~IGameState() = default;
    virtual void ProcessInput() = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
    virtual void Clean() = 0;
};

struct IGameStateDeleter
{
	void operator()( IGameState* gstate ) const
	{
        gstate->Clean();
		delete  gstate;
	}
};
