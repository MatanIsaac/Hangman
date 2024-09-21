#define SDL_MAIN_HANDLED 
#include <iostream>
#include "Game.hpp"

int main(int argc, char* argv[]) 
{
    Game* game = new Game();
    game->RunGame();
    delete game;
    return 0;
}