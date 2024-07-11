#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <SDL.h>

#include "Keyboard.h"
#include <iostream>


class dae::Keyboard::KeyboardImpl
{
public:
	KeyboardImpl(int const index)
	{
		ZeroMemory(&m_CurrentState, sizeof(Uint8*));
		auto currentState{ SDL_GetKeyboardState(nullptr) };
		CopyMemory(&m_CurrentState, &currentState, sizeof(Uint8*));

		if (index != -1)
		{
			m_PlayerNumber = index;
		}
	}

	void Update()
	{
		CopyMemory(m_LastState, m_CurrentState, SDL_NUM_SCANCODES * sizeof(Uint8));
		SDL_PumpEvents();
	}

	bool IsButtonPressed(const int button, const InputType& inputType) const 
	{
		switch (inputType)
		{
		case dae::InputType::PressedThisFrame:
			return (m_CurrentState[button] and not m_LastState[button]); 
		case dae::InputType::Held:
			return (m_CurrentState[button] and m_LastState[button]);
		case dae::InputType::ReleasedThisFrame:
			return (not m_CurrentState[button] and m_LastState[button]);
		default:
			return false;
		}
	}
	int GetPlayerNumber() const { return m_PlayerNumber; }
private:
	int m_PlayerNumber{};
	Uint8 m_LastState[SDL_NUM_SCANCODES];
	const Uint8* m_CurrentState;
};

dae::Keyboard::Keyboard(int const index)
{
	m_Impl = std::make_unique<KeyboardImpl>(index);
}

dae::Keyboard::~Keyboard()
{
}

void dae::Keyboard::Update()
{
	m_Impl->Update();
}

bool dae::Keyboard::IsButtonPressed(const int button, const InputType& inputType) const
{
	return m_Impl->IsButtonPressed(button, inputType);
}

int dae::Keyboard::GetPlayerNumber() const
{
	return m_Impl->GetPlayerNumber();
}
