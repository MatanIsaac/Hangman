#include "Button.hpp"
#include "../Util/ColorMacros.hpp"
#include <iostream>
#include <SDL_mouse.h>

Button::Button(SDL_Renderer* renderer,const char *buttonText, uint8_t textSize, const glm::vec2 &position)
	: m_IsButtonDown(false),
	  m_ButtonLocked(false),
	  m_ButtonText(buttonText),
	  m_ButtonSize(textSize),
	  m_Position(position),
	  m_OriginalPosition(position),
	  m_ElapsedTime(0.f)
{

	m_TextRenderer = std::unique_ptr<TextRenderer, TextRendererDeleter>
	(
    	new TextRenderer(renderer, "Assets/fonts/Filmcryptic.ttf", textSize),
    	TextRendererDeleter()
	);

	m_ButtonAction = ButtonAction::BUTTON_OUT;
}

void Button::ProcessInput()
{
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	
	if (!m_TextRenderer)
	{
		std::cout << "ERROR in Button class proccess input" << '\n';
		return;
	}

	const auto &buttonSize = m_TextRenderer->GetScale();

	bool isMouseHovering = (mouseX >= m_Position.x && mouseX <= (m_Position.x + buttonSize.x)) &&
                       		(mouseY >= m_Position.y && mouseY <= (m_Position.y + buttonSize.y));

	bool isLeftMousePressed = isMouseButtonPressed(SDL_BUTTON_LEFT);
	//bool isLeftMouseReleased = isMouseButtonReleased(SDL_BUTTON_LEFT);

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

void Button::Clean()
{
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

bool Button::isMouseButtonPressed(Uint32 button) const
{
	int x, y;
	Uint32 mouseState = SDL_GetMouseState(&x, &y);
	return (mouseState & SDL_BUTTON(button)) != 0;
}

bool Button::isMouseButtonReleased(Uint32 button) const
{
	return isMouseButtonPressed(button) == false;
}


void Button::ApplySineAnimation(float deltaTime,
								float frequency,
								float amplitude,
								float phaseOffset)
{
	m_ElapsedTime += deltaTime;

	// Apply easing by modifying the sine wave
	float easedSineWave = (1 - cos((m_ElapsedTime * frequency) + phaseOffset)) / 2 * amplitude;

	glm::vec2 buttonPos = GetOriginalPosition();
	buttonPos.y += easedSineWave;
	SetPosition(buttonPos);
}

void Button::SetPosition(const glm::vec2& newPosition) 
{
	m_TextRenderer->SetPosition(newPosition);
	m_Position = newPosition;
}