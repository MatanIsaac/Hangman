#pragma once
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <glm/glm.hpp>
#include <string>

namespace isaac_hangman
{
	struct SDLTextureDeleter
	{
		void operator()(SDL_Texture *sdl_t) const
		{
			SDL_DestroyTexture(sdl_t);
		}
	};

	class Texture
	{

	public:
		Texture();

		Texture(Texture &&other) noexcept;
		~Texture();

		bool CreateTexture(SDL_Renderer *renderer, std::string path);
		bool CreateTextureFromText(SDL_Renderer *renderer, TTF_Font *font, std::string textureText, SDL_Color textColor);

		void Render(SDL_Renderer *renderer, int xPos, int yPos);
		void RenderFrame(SDL_Renderer *renderer, int xPos, int yPos, SDL_Rect *clip, SDL_RendererFlip flip);

		void Clean();

		const glm::ivec2 &GetTextureSize() const { return mSize; }
		void SetTextureSize(const glm::ivec2 &newSize) { mSize = newSize; }

		void SetBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void SetTextureColor(SDL_Color newColor);

		SDL_Texture *GetTexture() const { return mTexture.get(); }

	private:
		std::unique_ptr<SDL_Texture, SDLTextureDeleter> mTexture;

		glm::ivec2 mSize;
	};

	struct TextureDeleter
	{
		void operator()(Texture *t) const
		{
			t->Clean();
			delete t;
		}
	};
}