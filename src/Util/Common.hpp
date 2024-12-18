#pragma once

constexpr int SCREEN_WIDTH = 800;
constexpr int SCREEN_HEIGHT = 600;

template <typename Object, void (*DeleterFunc)(Object*)>
struct Deleter
{
    void operator() (Object* obj) const noexcept
    {
        try {
            DeleterFunc(obj);
        } catch (...) 
        {
            fprintf(stderr, "Failed to delete Unique SDL Type!\n");
        }
    }
};

template <typename Object, void (*DeleterFunc)(Object*)>
using UniquePtr = std::unique_ptr<Object, Deleter<Object, DeleterFunc>>;

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

using Unique_SDL_Window     = UniquePtr<SDL_Window, SDL_DestroyWindow>;
using Unique_SDL_Renderer   = UniquePtr<SDL_Renderer, SDL_DestroyRenderer>;
using Unique_TTF_Font       = UniquePtr<TTF_Font, TTF_CloseFont>;
using Unique_Mixer_Chunk    = UniquePtr<Mix_Chunk, Mix_FreeChunk>;
using Unique_Mixer_Music    = UniquePtr<Mix_Music, Mix_FreeMusic>;
