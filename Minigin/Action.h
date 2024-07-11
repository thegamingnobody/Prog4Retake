#ifndef ACTION
#define ACTION

#include <memory>
#include "Command.h"
#include "ButtonEnums.h"
#include <SDL_scancode.h>

namespace dae
{
	class Action
	{
	public:
		enum class DeviceType 
		{
			Controller,
			Keyboard
		};

		Action(const ControllerButtons& controllerButton,	const InputType& inputType,	std::shared_ptr<Command> command, int const playerNumber);
		Action(const KeyboardKeys& keyboardKey,				const InputType& inputType,	std::shared_ptr<Command> command, int const playerNumber);

		void Execute();

		int GetButton()				const;
		int GetPlayerNumber()		const { return m_PlayerNumber; }
		DeviceType GetDeviceType()	const { return m_DeviceType; }
		InputType GetInputType()	const { return m_InputType; }


	private:
		int m_PlayerNumber;

		int m_ButtonValue;
		std::shared_ptr<Command> m_Command;
		DeviceType m_DeviceType;
		InputType m_InputType;
	};
}
#endif