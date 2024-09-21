#pragma once

#include <SDL.h>
#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "TextRenderer.hpp"

class Game;

enum class ButtonAction
{
	BUTTON_OUT,
	BUTTON_HOVERED,
	BUTTON_DOWN,
	BUTTON_PRESSED,
	BUTTON_RELEASED,
	BUTTON_TOTAL
};

class Button
{
public:
	Button(SDL_Renderer* renderer, const char *buttonText, uint8_t textSize, const glm::vec2 &position);

	void ProcessInput();
	void Render();
	void Clean();

	bool isMouseButtonPressed(Uint32 button) const;
	bool isMouseButtonReleased(Uint32 button) const;

	bool isButtonPressed() const { return m_IsButtonPressed; }

	void ResetButton() { m_IsButtonPressed = false; }

	const glm::vec2& GetOriginalPosition() const { return m_OriginalPosition; }
	const glm::vec2& GetPosition() const { return m_Position; }
	void SetPosition(const glm::vec2& newPosition);

 	void ApplySineAnimation(float deltaTime, float frequency, float amplitude, float phaseOffset);

	uint8_t GetButtonSize() const { return m_ButtonSize; }

	bool GetButtonLocked() const { return m_ButtonLocked; }	
	void SetButtonLock(bool lock) {  m_ButtonLocked = lock; }	

	const char* GetButtonText() const { return m_ButtonText.c_str(); }
private:
	bool m_IsButtonPressed = false;
	bool m_ButtonLocked = false;

	TextRenderer* m_TextRenderer;

	std::string m_ButtonText;
	uint8_t m_ButtonSize;
	glm::vec2 m_Position;
	glm::vec2 m_OriginalPosition;

	ButtonAction m_ButtonAction;

	float m_ElapsedTime = 0.0f;
};

struct ButtonDeleter
{
	void operator()(Button *b) const
	{
		b->Clean();
		delete b;
	}
};
