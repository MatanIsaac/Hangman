#include "Button.hpp"
#include <iostream>
#include <SDL_mouse.h>
#include "../Util/ColorMacros.hpp"
#include "Core/SoundManager.hpp"

namespace isaac_hangman
{
	Button::Button(const char *buttonText, const glm::vec2 &position)
		: 
		m_ButtonAction(ButtonAction::BUTTON_OUT),
		m_ButtonText(buttonText),
		m_ButtonSize({0.f,0.f}),
		m_Position(position),
		m_OriginalPosition(position),
		m_ElapsedTime(0.0f),
		m_ButtonDown(false),
		m_ButtonLocked(false)
	{
		m_ButtonSize = TextRenderer::GetInstance().GetTextDimensions(m_ButtonText);
	}

	Button::Button(const Button& btn)
	{
		m_ButtonAction		= btn.m_ButtonAction;
		m_ButtonText		= btn.m_ButtonText;
		m_ButtonSize		= btn.m_ButtonSize;
		m_Position			= btn.m_Position;
		m_OriginalPosition	= btn.m_OriginalPosition;
		m_ButtonDown 		= btn.m_ButtonDown;
		m_ButtonLocked 		= btn.m_ButtonLocked;

	}

	Button::Button(Button&& other) noexcept
		: 
		m_ButtonAction(other.m_ButtonAction),
		m_ButtonText(std::move(other.m_ButtonText)), 
		m_ButtonSize(std::move(other.m_ButtonSize)), 
		m_Position(std::move(other.m_Position)),
		m_OriginalPosition(std::move(other.m_OriginalPosition)),
		m_ElapsedTime(other.m_ElapsedTime),
		m_ButtonDown(other.m_ButtonDown),
		m_ButtonLocked(other.m_ButtonLocked)
	{
		
	}

	Button& Button::operator=(Button&& other) noexcept
	{
		if (this != &other) 
		{
			m_ButtonAction 		= other.m_ButtonAction;
			m_ButtonText 		= std::move(other.m_ButtonText);
			m_ButtonSize 		= std::move(other.m_ButtonSize);
			m_Position 			= std::move(other.m_Position);
			m_OriginalPosition 	= std::move(other.m_OriginalPosition);
			m_ElapsedTime 		= other.m_ElapsedTime;
			m_ButtonDown 		= other.m_ButtonDown;
			m_ButtonLocked 		= other.m_ButtonLocked;
		}
		return *this;
	}

    Button& Button::operator=(const Button& other)
	{
		if (this != &other) 
		{
			m_ButtonAction 		= other.m_ButtonAction;
			m_ButtonText 		= other.m_ButtonText;
			m_ButtonSize 		= other.m_ButtonSize;
			m_Position 			= other.m_Position;
			m_OriginalPosition 	= other.m_OriginalPosition;
			m_ElapsedTime 		= other.m_ElapsedTime;
			m_ButtonDown 		= other.m_ButtonDown;
			m_ButtonLocked 		= other.m_ButtonLocked;
		}
		return *this;
	}

	void Button::Update(float deltaTime)
	{
		int mouseX, mouseY;
		Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		// Check if the mouse is hovering over the button
		bool isMouseHovering = (mouseX >= m_Position.x && mouseX <= (m_Position.x + m_ButtonSize.x)) &&
							(mouseY >= m_Position.y && mouseY <= (m_Position.y + m_ButtonSize.y));

		// Check if the left mouse button is pressed
		bool isLeftMousePressed = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);

		if (isMouseHovering)
		{
			if(!m_PlayOnce)
			{
				SoundManager::GetInstance().PlayRandomSFX();
				m_PlayOnce = true;
			}

			if (isLeftMousePressed)
			{
				if (!m_ButtonLocked)
				{
					m_ButtonAction = ButtonAction::BUTTON_PRESSED;
					m_ButtonLocked = true; // Lock on press
				}
				else
				{
					m_ButtonAction = ButtonAction::BUTTON_DOWN;
				}
				m_ButtonDown = true; // Set button as held down
			}
			else
			{
				if (m_ButtonDown)
				{
					m_ButtonAction = ButtonAction::BUTTON_RELEASED; // Trigger button release
				}
				else
				{
					m_ButtonAction = ButtonAction::BUTTON_HOVERED; // Hovered but not pressed
				}
				m_ButtonDown = false; // Reset button down state
			}
		}
		else
		{
			m_PlayOnce = false;
			if (isLeftMousePressed)
			{
				// Mouse pressed outside the button
				m_ButtonLocked = true;  // Lock to prevent accidental activation on hover
			}

			if (!isLeftMousePressed)
			{
				// Mouse released outside
				m_ButtonLocked = false; // Unlock when released
			}

			// Set action to out when the mouse is outside the button
			m_ButtonAction = ButtonAction::BUTTON_OUT;
			m_ButtonDown = false;
		}
	}

	void Button::Render(SDL_Renderer* renderer)
	{
		auto& textRenderer = TextRenderer::GetInstance();
		
		switch (m_ButtonAction)
		{
		case ButtonAction::BUTTON_OUT:
			textRenderer.RenderText(renderer,m_Position.x, m_Position.y, SDL_Color{255, 183, 77, 255}, m_ButtonText);
			break;

		case ButtonAction::BUTTON_HOVERED:
			textRenderer.RenderText(renderer,m_Position.x, m_Position.y, SDL_Color{255, 255, 255, 255}, m_ButtonText);
			break;

		case ButtonAction::BUTTON_PRESSED:
			textRenderer.RenderText(renderer,m_Position.x, m_Position.y, SDL_Color{0, 0, 255, 255}, m_ButtonText);
			break;

		case ButtonAction::BUTTON_DOWN:
			textRenderer.RenderText(renderer,m_Position.x, m_Position.y, SDL_Color{0, 0, 255, 255}, m_ButtonText);
			break;

		case ButtonAction::BUTTON_RELEASED:
			textRenderer.RenderText(renderer,m_Position.x, m_Position.y, SDL_Color{255, 0, 0, 255}, m_ButtonText);
			break;

		default:
			textRenderer.RenderText(renderer,m_Position.x, m_Position.y, SDL_Color{255, 183, 77, 255}, m_ButtonText);
			break;
		}
	}


	bool Button::isPressed()
	{
		// Check if the button is hovered and was pressed
		if (m_ButtonAction == ButtonAction::BUTTON_RELEASED && m_ButtonLocked)
		{
			// Unlock the button and register the click
			m_ButtonLocked = false;
			return true;
		}

		// Do not register a click otherwise
		return false;
	}


	void Button::ApplySineAnimation(float deltaTime, float frequency, float amplitude, float phaseOffset)
	{
		m_ElapsedTime += deltaTime;

		// Apply easing by modifying the sine wave
		float easedSineWave = (1 - cos((m_ElapsedTime * frequency) + phaseOffset)) / 2 * amplitude;

		glm::vec2 buttonPos = GetOriginalPosition();
		buttonPos.y 		+= easedSineWave;
		SetPosition(buttonPos);
	}

	void Button::SetPosition(const glm::vec2& newPosition) 
	{
		m_Position = newPosition;
	}

}