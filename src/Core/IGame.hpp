#pragma once
namespace isaac_hangman
{
    class IGame
    {
    public:
        virtual ~IGame() = default;
        virtual void Quit() = 0;
    };
}
