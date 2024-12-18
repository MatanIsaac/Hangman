#include "Texture.hpp"
#include <iostream>

namespace isaac_hangman
{
	Texture::Texture()
		: m_Texture(nullptr)
		, m_Size(glm::ivec2())
	{}

	Texture::Texture(Texture &&other) noexcept

		: m_Texture(std::move(other.m_Texture)), m_Size(other.m_Size)
	{
		other.SetTextureSize(glm::ivec2(0, 0));
	}

	bool Texture::CreateTexture(std::string path,SDL_Renderer* renderer)
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

		SDL_QueryTexture(newTexture, nullptr, nullptr, &m_Size.x, &m_Size.y);

		m_Texture = UniqueSDLTexture(newTexture,SDL_DestroyTexture);
		return (m_Texture != nullptr);
	}

	bool Texture::CreateTextureFromText(TTF_Font* font, std::string textureText, SDL_Color textColor,SDL_Renderer* renderer)
	{
		SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
		if (loadedSurface == nullptr)
		{
			std::cout << "Unable to render text: " << textureText.c_str() << " Error: " << TTF_GetError() << '\n';
			return false;
		}

		m_Texture = UniqueSDLTexture(SDL_CreateTextureFromSurface(renderer, loadedSurface),SDL_DestroyTexture);	
		if (m_Texture == nullptr)
		{
			std::cout << "Failed to create texture from text: " << textureText.c_str() << " Error: " << SDL_GetError() << '\n';
			SDL_FreeSurface(loadedSurface); // Free the loaded surface
			return false;
		}

		// Query the texture to get the width and height
		if (SDL_QueryTexture(m_Texture.get(), nullptr, nullptr, &m_Size.x, &m_Size.y) != 0)
		{
			std::cout << "Failed to query texture, Error: " << SDL_GetError() << '\n';
			SDL_FreeSurface(loadedSurface); // Free the loaded surface
			return false;
		}

		SDL_FreeSurface(loadedSurface); // Free the loaded surface
		return (m_Texture != nullptr);
	}
	void Texture::Render(SDL_Renderer* renderer,int xPos, int yPos)
	{
		SDL_Rect srcRect = {0, 0, m_Size.x, m_Size.y};
		SDL_Rect renderQuad = {xPos, yPos, m_Size.x, m_Size.y};
		SDL_RenderCopyEx(renderer, m_Texture.get(), &srcRect, &renderQuad, 0.0, nullptr, SDL_FLIP_NONE);
	}

	void Texture::SetBlendMode(SDL_BlendMode blending)
	{
		SDL_SetTextureBlendMode(m_Texture.get(), blending);
	}

	void Texture::setAlpha(Uint8 alpha)
	{
		SDL_SetTextureAlphaMod(m_Texture.get(), alpha);
	}
	
	void Texture::SetTextureColor(SDL_Color newColor)
	{
		SDL_SetTextureColorMod(m_Texture.get(), newColor.r, newColor.g, newColor.b);
	}

}