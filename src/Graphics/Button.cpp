#include "Button.hpp"
#include "../Util/ColorMacros.hpp"
#include <iostream>
#include <SDL_mouse.h>

namespace isaac_hangman
{
	Button::Button(const char *buttonText, uint8_t textSize, const glm::vec2 &position)
		: m_TextRenderer(std::make_unique<TextRenderer>(35)),
		m_ButtonAction(ButtonAction::BUTTON_OUT),
		m_ButtonText(buttonText),
		m_ButtonSize(textSize),
		m_Position(position),
		m_OriginalPosition(position),
		m_ElapsedTime(0.0f),
		m_IsButtonDown(false),
		m_ButtonLocked(false)
	{
	}

	Button::Button(const Button& btn)
	{
		m_TextRenderer 		= btn.m_TextRenderer->Clone();
		m_ButtonAction		= btn.m_ButtonAction;
		m_ButtonText		= btn.m_ButtonText;
		m_ButtonSize		= btn.m_ButtonSize;
		m_Position			= btn.m_Position;
		m_OriginalPosition	= btn.m_OriginalPosition;
		m_IsButtonDown 		= btn.m_IsButtonDown;
		m_ButtonLocked 		= btn.m_ButtonLocked;

	}

	// Move Constructor
	Button::Button(Button&& other) noexcept
		: m_TextRenderer(std::move(other.m_TextRenderer)), // Transfer ownership
		m_ButtonAction(other.m_ButtonAction),
		m_ButtonText(std::move(other.m_ButtonText)), // Move string
		m_ButtonSize(other.m_ButtonSize),
		m_Position(std::move(other.m_Position)),
		m_OriginalPosition(std::move(other.m_OriginalPosition)),
		m_ElapsedTime(other.m_ElapsedTime),
		m_IsButtonDown(other.m_IsButtonDown),
		m_ButtonLocked(other.m_ButtonLocked)
	{
		// Nothing else to do, as moved-from resources are now owned here
	}

	// Move Assignment Operator
	Button& Button::operator=(Button&& other) noexcept
	{
		if (this != &other) // Check for self-assignment
		{
			// Transfer ownership of unique_ptr
			m_TextRenderer = std::move(other.m_TextRenderer);

			// Move other members
			m_ButtonAction 		= other.m_ButtonAction;
			m_ButtonText 		= std::move(other.m_ButtonText);
			m_ButtonSize 		= other.m_ButtonSize;
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
		if (this != &other) // Check for self-assignment
		{
			// Copy non-pointer members
			m_ButtonAction 		= other.m_ButtonAction;
			m_ButtonText 		= other.m_ButtonText;
			m_ButtonSize 		= other.m_ButtonSize;
			m_Position 			= other.m_Position;
			m_OriginalPosition 	= other.m_OriginalPosition;
			m_ElapsedTime 		= other.m_ElapsedTime;
			m_IsButtonDown 		= other.m_IsButtonDown;
			m_ButtonLocked 		= other.m_ButtonLocked;

			// Use the Clone method for TextRenderer
			if (other.m_TextRenderer)
			{
				m_TextRenderer = other.m_TextRenderer->Clone();
			}
			else
			{
				m_TextRenderer.reset();
			}
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

		if (!m_TextRenderer)
		{
			std::cout << "ERROR in Button class proccess input" << '\n';
			return;
		}

		const auto &buttonSize 	= m_TextRenderer->GetScale();

		bool isMouseHovering 	= (mouseX >= m_Position.x && mouseX <= (m_Position.x + buttonSize.x)) &&
								(mouseY >= m_Position.y && mouseY <= (m_Position.y + buttonSize.y));

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
		if(m_TextRenderer)
		{
			if (m_ButtonAction == ButtonAction::BUTTON_OUT )
			{
				m_TextRenderer->RenderText(m_Position.x, m_Position.y, SDL_Color{255, 183, 77, 255} ,m_ButtonText);
			}
			else if (m_ButtonAction == ButtonAction::BUTTON_HOVERED )
			{
				m_TextRenderer->RenderText(m_Position.x, m_Position.y, SDL_Color {255,255,255,255} ,m_ButtonText);
				
			}
			else if (m_ButtonAction == ButtonAction::BUTTON_PRESSED)
			{
				m_TextRenderer->RenderText(m_Position.x, m_Position.y, SDL_Color {0,0,255,255} ,m_ButtonText);
			}
			else
			{
				// Default rendering if other states aren't met
				m_TextRenderer->RenderText(m_Position.x, m_Position.y, SDL_Color{255, 183, 77, 255} ,m_ButtonText);
			}
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
		m_TextRenderer->SetPosition(newPosition);
		m_Position = newPosition;
	}

}