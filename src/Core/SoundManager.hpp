#include <SDL2/SDL_mixer.h>
#include <string>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>

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
        bool init(int frequency = MIX_DEFAULT_FREQUENCY, Uint16 format = MIX_DEFAULT_FORMAT, int channels = 2, int chunksize = 2048) 
        {
            if (Mix_OpenAudio(frequency, format, channels, chunksize) < 0) 
            {
                printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                return false;
            }
         
		    std::string base = "D:\\SourceControl\\Hangman\\Assets\\SFX\\Chalkboard_SFX-0";
		    for(int i = 1; i < 8; i++)
		    {
			    std::string final_path = base + std::to_string(i) + ".wav";
                std::string sfx_id = "sfx_" + std::to_string(i); 
			    if(!loadSFX(sfx_id,final_path.c_str()))
			    {
				    printf( "Failed to load sfx_id: %s! SDL_mixer Error: %s\n",sfx_id.c_str(), Mix_GetError() );
			    }
		    }
            return true;
        }

        // Load a sound effect
        bool loadSFX(const std::string& id, const std::string& path) 
        {
            Mix_Chunk* sfx = Mix_LoadWAV(path.c_str());
            if (!sfx) 
            {
                printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
                return false;
            }
            m_SFX[id] = sfx;
            return true;
        }

        // Load music
        bool loadMusic(const std::string& id, const std::string& path) 
        {
            Mix_Music* music = Mix_LoadMUS(path.c_str());
            if (!music) 
            {
                printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
                return false;
            }
            m_Music[id] = music;
            return true;
        }

        // Play a sound effect
        void playSFX(const std::string& id, int loops = 0) 
        {
            auto it = m_SFX.find(id);
            if (it != m_SFX.end()) 
            {
                Mix_PlayChannel(-1, it->second, loops);
            } 
            else 
            {
                printf("Sound effect with ID '%s' not found!\n", id.c_str());
            }
        }

        // Play music
        void playMusic(const std::string& id, int loops = -1) 
        {
            auto it = m_Music.find(id);
            if (it != m_Music.end()) 
            {
                Mix_PlayMusic(it->second, loops);
            } 
            else 
            {
                printf("Music with ID '%s' not found!\n", id.c_str());
            }
        }

        // Play a random sound effect
        void playRandomSFX() 
        {
            if (m_SFX.empty()) 
            {
                printf("No sound effects loaded to play randomly!\n");
                return;
            }

            int randomIndex = rand() % m_SFX.size();
            auto it = std::next(m_SFX.begin(), randomIndex);
            Mix_PlayChannel(-1, it->second, 0);
        }

        // Stop music
        void stopMusic() 
        {
            Mix_HaltMusic();
        }

        // Clean up resources
        void cleanup() 
        {
            for (auto& pair : m_SFX) 
            {
                Mix_FreeChunk(pair.second);
            }
            m_SFX.clear();

            for (auto& pair : m_Music) 
            {
                Mix_FreeMusic(pair.second);
            }
            m_Music.clear();

            Mix_CloseAudio();
        }

    private:
        SoundManager()
        {
            init();
        }
        ~SoundManager() 
        {
            cleanup();
        }

        std::unordered_map<std::string, Mix_Chunk*> m_SFX;
        std::unordered_map<std::string, Mix_Music*> m_Music;
    };

} // namespace isaac_hangman
