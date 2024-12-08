#pragma once

#include <SDL.h>
#include <memory>
#include <glm/glm.hpp>
#include <string>
#include "TextRenderer.hpp"

namespace isaac_hangman
{
	enum class ButtonAction
	{
		BUTTON_OUT,
		BUTTON_HOVERED,
		BUTTON_DOWN,
		BUTTON_PRESSED,
		BUTTON_RELEASED,
		BUTTON_TOTAL
	};

	using UniqueButton = std::unique_ptr<class Button>;

	class Button
	{
	public:
		Button(const char *buttonText, const glm::vec2 &position);
		~Button() = default;
		Button(const Button& btn);
		Button(Button&& other) noexcept;
		Button& operator=(const Button& other);
		Button& operator=(Button&& other) noexcept;

		void Update(float deltaTime);
		void Render();

		bool isPressed();
		
		bool GetButtonLocked() const { return m_ButtonLocked; }	
		const char* GetButtonText() const { return m_ButtonText.c_str(); }
		const glm::vec2& GetButtonSize() const { return m_ButtonSize; }
		const glm::vec2& GetPosition() const { return m_Position; }
		
		void SetPosition(const glm::vec2& newPosition);

		void ApplySineAnimation(float deltaTime, float frequency, float amplitude, float phaseOffset);
	private:
		bool isButtonDown() const { return m_ButtonDown; }

		void ResetButton() { m_ButtonDown = false; }
		void SetButtonLock(bool lock) {  m_ButtonLocked = lock; }	

		const glm::vec2& GetOriginalPosition() const { return m_OriginalPosition; } /* used for sine animations */

	private: 
        ButtonAction 		m_ButtonAction; 		
        std::string 		m_ButtonText; 	
		glm::vec2 			m_ButtonSize;
        glm::vec2 			m_Position; 			
        glm::vec2 			m_OriginalPosition; 	
        float 				m_ElapsedTime; 		
        bool 				m_ButtonDown; 		
        bool 				m_ButtonLocked; 		
	};
}