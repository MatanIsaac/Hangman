#define SDL_MAIN_HANDLED 

#include "Game.hpp"

int main(int argc, char* argv[]) 
{
    Game* game = new Game();
    game->RunGame();
    return 0;
}