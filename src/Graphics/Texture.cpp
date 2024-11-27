#include "Texture.hpp"
#include <iostream>

namespace isaac_hangman
{
	Texture::Texture()
		: mTexture(nullptr), mSize(glm::ivec2())
	{
	}

	Texture::Texture(Texture &&other) noexcept

		: mTexture(std::move(other.mTexture)), mSize(other.mSize)
	{
		other.SetTextureSize(glm::ivec2(0, 0));
	}

	Texture::~Texture() { Clean(); }

	bool Texture::CreateTexture(SDL_Renderer* renderer, std::string path)
	{
		SDL_Surface* loadedSurface = IMG_Load(path.c_str());
		if (loadedSurface == nullptr)
		{
			std::cout << "Unable to load image: " << path.c_str() << " Error: " << IMG_GetError() << '\n';
			return false;
		}

		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

		SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == nullptr)
		{
			std::cout << "Failed to create texture from: " << path.c_str() << " Error: " << SDL_GetError() << '\n';
			SDL_FreeSurface(loadedSurface); // Free the loaded surface
			return false;
		}
		SDL_FreeSurface(loadedSurface); // Free the loaded surface after texture creation

		SDL_QueryTexture(newTexture, nullptr, nullptr, &mSize.x, &mSize.y);

		mTexture.reset(newTexture);
		return (mTexture != nullptr);
	}

	bool Texture::CreateTextureFromText(SDL_Renderer* renderer, TTF_Font* font, std::string textureText, SDL_Color textColor)
	{
		SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
		if (loadedSurface == nullptr)
		{
			std::cout << "Unable to render text: " << textureText.c_str() << " Error: " << TTF_GetError() << '\n';
			return false;
		}

		mTexture.reset(SDL_CreateTextureFromSurface(renderer, loadedSurface));
		if (mTexture == nullptr)
		{
			std::cout << "Failed to create texture from text: " << textureText.c_str() << " Error: " << SDL_GetError() << '\n';
			SDL_FreeSurface(loadedSurface); // Free the loaded surface
			return false;
		}

		// Query the texture to get the width and height
		if (SDL_QueryTexture(mTexture.get(), nullptr, nullptr, &mSize.x, &mSize.y) != 0)
		{
			std::cout << "Failed to query texture, Error: " << SDL_GetError() << '\n';
			SDL_FreeSurface(loadedSurface); // Free the loaded surface
			return false;
		}

		SDL_FreeSurface(loadedSurface); // Free the loaded surface
		return (mTexture != nullptr);
	}
	void Texture::Render(SDL_Renderer *renderer, int xPos, int yPos)
	{
		SDL_Rect srcRect = {0, 0, mSize.x, mSize.y};
		SDL_Rect renderQuad = {xPos, yPos, mSize.x, mSize.y};
		SDL_RenderCopyEx(renderer, mTexture.get(), &srcRect, &renderQuad, 0.0, nullptr, SDL_FLIP_NONE);
	}

	void Texture::RenderFrame(SDL_Renderer *renderer, int xPos, int yPos, SDL_Rect *clip, SDL_RendererFlip flip)
	{
		// Set rendering space and render to screen
		SDL_Rect renderQuad = {xPos, yPos, mSize.x, mSize.y};

		// Set clip rendering dimensions
		if (clip != nullptr)
		{
			renderQuad.w = clip->w;
			renderQuad.h = clip->h;
		}

		// Render to screen
		SDL_RenderCopyEx(renderer, mTexture.get(), clip, &renderQuad, 0.0, nullptr, flip);
	}

	void Texture::Clean()
	{
	}

	void Texture::SetBlendMode(SDL_BlendMode blending)
	{
		SDL_SetTextureBlendMode(mTexture.get(), blending);
	}

	void Texture::setAlpha(Uint8 alpha)
	{
		SDL_SetTextureAlphaMod(mTexture.get(), alpha);
	}
	void Texture::SetTextureColor(SDL_Color newColor)
	{
		SDL_SetTextureColorMod(mTexture.get(), newColor.r, newColor.g, newColor.b);
	}

}