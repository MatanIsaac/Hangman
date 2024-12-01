#include "Button.hpp"
#include "../Util/ColorMacros.hpp"
#include <iostream>
#include <SDL_mouse.h>

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
		m_IsButtonDown(false),
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
		m_IsButtonDown 		= btn.m_IsButtonDown;
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
		m_IsButtonDown(other.m_IsButtonDown),
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
			m_IsButtonDown 		= other.m_IsButtonDown;
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
			m_IsButtonDown 		= other.m_IsButtonDown;
			m_ButtonLocked 		= other.m_ButtonLocked;
		}
		return *this;
	}


    void Button::ProcessInput()
	{
		
	}

	void Button::Update(float deltaTime)
	{
		int mouseX, mouseY;
		Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY); 

    	bool isMouseHovering = (mouseX >= m_Position.x && mouseX <= (m_Position.x + m_ButtonSize.x)) &&
                           		(mouseY >= m_Position.y && mouseY <= (m_Position.y + m_ButtonSize.y));

		bool isLeftMousePressed = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);

		if (isMouseHovering)
		{
			if (isLeftMousePressed)
			{
				if (!m_IsButtonDown)
				{
					m_ButtonAction = ButtonAction::BUTTON_PRESSED;
					m_IsButtonDown = true;
				}
			}
			else if (m_IsButtonDown)
			{
				m_ButtonAction = ButtonAction::BUTTON_RELEASED;
				m_IsButtonDown = false;
			}
			else
			{
				m_ButtonAction = ButtonAction::BUTTON_HOVERED;
			}
		}
		else
		{
			m_ButtonAction = ButtonAction::BUTTON_OUT;
		}
	}

	void Button::Render()
	{
		auto& textRenderer = TextRenderer::GetInstance();
		
		if (m_ButtonAction == ButtonAction::BUTTON_OUT )
		{
			textRenderer.RenderText(m_Position.x, m_Position.y, SDL_Color{255, 183, 77, 255} ,m_ButtonText);
		}
		else if (m_ButtonAction == ButtonAction::BUTTON_HOVERED )
		{
			textRenderer.RenderText(m_Position.x, m_Position.y, SDL_Color {255,255,255,255} ,m_ButtonText);
			
		}
		else if (m_ButtonAction == ButtonAction::BUTTON_PRESSED)
		{
			textRenderer.RenderText(m_Position.x, m_Position.y, SDL_Color {0,0,255,255} ,m_ButtonText);
		}
		else
		{
			// Default rendering if other states aren't met
			textRenderer.RenderText(m_Position.x, m_Position.y, SDL_Color{255, 183, 77, 255} ,m_ButtonText);
		}
		
	}

	bool Button::isPressed()
	{
		if (isButtonDown() && !GetButtonLocked())
		{
			SetButtonLock(true);
		}
		else if (!isButtonDown() && GetButtonLocked())
		{
			SetButtonLock(false);
			return true;	
		}
		
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
		auto& textRenderer = TextRenderer::GetInstance();
		textRenderer.SetPosition(newPosition);
		m_Position = newPosition;
	}

}