#include "Action.h"

dae::Action::Action(const ControllerButtons& controllerButton, const InputType& inputType, std::shared_ptr<Command> command, int const playerNumber) 
	: m_PlayerNumber	(playerNumber)
	, m_ButtonValue		(static_cast<int>(controllerButton))
	, m_Command			(command)
	, m_DeviceType		(DeviceType::Controller)
	, m_InputType		(inputType)
{
}

dae::Action::Action(const KeyboardKeys& keyboardKey, const InputType& inputType, std::shared_ptr<Command> command, int const playerNumber) 
	: m_PlayerNumber	(playerNumber)
	, m_ButtonValue		(static_cast<int>(keyboardKey))
	, m_Command			(command)
	, m_DeviceType		(DeviceType::Keyboard)
	, m_InputType		(inputType)
{
}

void dae::Action::Execute()
{
	m_Command->Execute();
}

int dae::Action::GetButton() const
{
	return m_ButtonValue;
}
