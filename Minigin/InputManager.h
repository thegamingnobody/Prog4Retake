#pragma once
#include "Singleton.h"
#include <vector>
#include <memory>
#include "InputDevice.h"
#include "Action.h"

namespace dae
{
	class InputManager final : public Singleton<InputManager>
	{
	public:
		bool ProcessInput();

		int AddInputDevice(const Action::DeviceType& DeviceType);

		std::shared_ptr<Action> AddAction(const ControllerButtons& controllerButton, const InputType& inputType, std::shared_ptr<Command> command, int const playerNumber);
		std::shared_ptr<Action> AddAction(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const playerNumber);

	private:
		std::vector<std::unique_ptr<InputDevice>> m_InputDevices;
		std::vector<std::shared_ptr<Action>> m_Actions;

	};

}
