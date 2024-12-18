#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <filesystem>
#include <memory>
#include <sstream>
#include "Util/Common.hpp"

namespace isaac_hangman
{
    class SoundManager 
    {
    public:
        // Singleton access
        static SoundManager& GetInstance() 
        {
            static SoundManager instance;
            return instance;
        }

        // Prevent copy construction and assignment
        SoundManager(const SoundManager&) = delete;
        SoundManager& operator=(const SoundManager&) = delete;

        // Initialize SDL_mixer with desired audio format
        bool Init(int frequency = MIX_DEFAULT_FREQUENCY, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 2048) 
        {
            if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) 
            {
                printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                return false;
            }

            std::string basePath = std::filesystem::current_path().string() + "\\Assets\\SFX\\Chalkboard_SFX-0";
            
            for (int i = 1; i < 8; ++i) 
            {
                std::ostringstream finalPathStream;
                finalPathStream << basePath << i << ".wav";
                std::string finalPath = finalPathStream.str();

                std::ostringstream sfxIDStream;
                sfxIDStream << "sfx_" << i;
                std::string sfxID = sfxIDStream.str();

                if (!LoadSFX(sfxID, finalPath)) 
                {
                    printf("Failed to load sfx_id: %s! SDL_mixer Error: %s\n", sfxID.c_str(), Mix_GetError());
                }
            }

            return true;
        }

        // Load a sound effect
        bool LoadSFX(const std::string& id, const std::string& path) 
        {
            Unique_Mixer_Chunk sfx(Mix_LoadWAV(path.c_str()));
            if (!sfx) 
            {
                printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
                return false;
            }
            m_SFX[id] = std::move(sfx);
            return true;
        }

        // Load music
        bool LoadMusic(const std::string& id, const std::string& path) 
        {
            Unique_Mixer_Music music(Mix_LoadMUS(path.c_str()));
            if (!music) 
            {
                printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
                return false;
            }
            m_Music[id] = std::move(music);
            return true;
        }

        // Play a sound effect
        void PlaySFX(const std::string& id, int loops = 0) 
        {
            auto it = m_SFX.find(id);
            if (it != m_SFX.end()) 
            {
                Mix_PlayChannel(-1, it->second.get(), loops);
            } 
            else 
            {
                printf("Sound effect with ID '%s' not found!\n", id.c_str());
            }
        }

        // Play music
        void PlayMusic(const std::string& id, int loops = -1) 
        {
            auto it = m_Music.find(id);
            if (it != m_Music.end()) 
            {
                Mix_PlayMusic(it->second.get(), loops);
            } 
            else 
            {
                printf("Music with ID '%s' not found!\n", id.c_str());
            }
        }

        // Play a random sound effect
        void PlayRandomSFX() 
        {
            if (m_SFX.empty()) 
            {
                printf("No sound effects loaded to play randomly!\n");
                return;
            }

            int randomIndex = rand() % m_SFX.size();
            auto it = std::next(m_SFX.begin(), randomIndex);
            Mix_PlayChannel(-1, it->second.get(), 0);
        }

        // Stop music
        void StopMusic() 
        {
            Mix_HaltMusic();
        }

        // Clean up resources
        void Clean() 
        {
            m_SFX.clear();
            m_Music.clear();
            Mix_CloseAudio();
        }

    private:
        SoundManager() = default;
        ~SoundManager() { Clean(); }
        
        std::unordered_map<std::string, Unique_Mixer_Chunk> m_SFX;
        std::unordered_map<std::string, Unique_Mixer_Music> m_Music;
    };

} // namespace isaac_hangman
