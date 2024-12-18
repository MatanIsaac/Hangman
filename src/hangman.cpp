#include <iostream>
#include "Core/Game.hpp"

using namespace isaac_hangman;

int main(int argc, char* argv[]) 
{
    Game* game = new Game();
    game->Run();
    delete game;
    return 0;
}