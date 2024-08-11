#include "CountersCommand.h"
#include "Event.h"
#include <string>
#include "EventManager.h"

dae::CountersCommand::CountersCommand(int const playerNumber, const std::string& targetCounterName, int const addedValue)
	: m_TargetCounterName(targetCounterName)
	, m_AddedValue(addedValue)
{
	m_PlayerNumber = playerNumber;
}

void dae::CountersCommand::Execute()
{
	std::tuple<const std::string&, int const> eventArguments{ m_TargetCounterName, m_AddedValue };
	Event eventToNotify{ dae::EventType::UpdateCounter, eventArguments, m_PlayerNumber };

	dae::EventManager::GetInstance().PushEvent(eventToNotify);
}