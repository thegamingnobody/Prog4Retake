#include <SDL.h>
#include "InputManager.h"
#include "Controller.h"
#include "Keyboard.h"

bool dae::InputManager::ProcessInput()
{
	for (auto& inputDevice : m_InputDevices)
	{
		inputDevice->Update();
	}

	for (auto& action : m_Actions)
	{
		for (auto& inputDevice : m_InputDevices)
		{
			if (inputDevice->GetPlayerNumber() != action->GetPlayerNumber()) continue;

			if (inputDevice->IsButtonPressed(action->GetButton(), action->GetInputType()))
			{
				action->Execute();
			}
		}
	}

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		if (e.type == SDL_QUIT) {
			return false;
		}
	}

	return true;
}

int dae::InputManager::AddInputDevice(const Action::DeviceType& DeviceType)
{
	int const newPlayerNumber{ static_cast<int>(m_InputDevices.size()) };

	switch (DeviceType)
	{
	case Action::DeviceType::Controller:
		m_InputDevices.emplace_back(std::make_unique<Controller>(newPlayerNumber));
		break;
	case Action::DeviceType::Keyboard:
		m_InputDevices.emplace_back(std::make_unique<Keyboard>(newPlayerNumber));
		break;
	default:
		break;
	}

	return newPlayerNumber;

}

std::shared_ptr<dae::Action> dae::InputManager::AddAction(const ControllerButtons& controllerButton, const InputType& inputType, std::shared_ptr<Command> command, int const playerNumber)
{
	std::shared_ptr<Action> action = std::make_shared<Action>(controllerButton, inputType, command, playerNumber);
	m_Actions.emplace_back(action);
	return action;
}

std::shared_ptr<dae::Action> dae::InputManager::AddAction(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const playerNumber)
{
	std::shared_ptr<Action> action = std::make_shared<Action>(keyboardKey, inputType, command, playerNumber);
	m_Actions.emplace_back(action);
	return action;
}
