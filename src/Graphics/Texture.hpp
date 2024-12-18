#pragma once
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <memory>
#include <functional>
#include <glm/glm.hpp>
#include <string>

namespace isaac_hangman
{
	
	class Texture
	{
	using UniqueSDLTexture    = std::unique_ptr<SDL_Texture,std::function<void(SDL_Texture *)>>;

	public:
		Texture();

		Texture(Texture &&other) noexcept;
		~Texture() = default;

		bool CreateTexture(std::string path,SDL_Renderer* renderer);
		bool CreateTextureFromText(TTF_Font *font, std::string textureText, SDL_Color textColor,SDL_Renderer* renderer);

		void Render(SDL_Renderer* renderer,int xPos, int yPos);

		const glm::ivec2 &GetTextureSize() const { return m_Size; }
		void SetTextureSize(const glm::ivec2 &newSize) { m_Size = newSize; }

		void SetBlendMode(SDL_BlendMode blending);
		void setAlpha(Uint8 alpha);
		void SetTextureColor(SDL_Color newColor);

		SDL_Texture *GetTexture() const { return m_Texture.get(); }

	private:
		UniqueSDLTexture m_Texture;
		glm::ivec2 m_Size;
	};

	using Unique_Texture = std::unique_ptr<Texture>;
}