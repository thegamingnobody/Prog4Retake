#include "SkipLevelCommand.h"
#include "Event.h"
#include "EventManager.h"

void dae::SkipLevelCommand::Execute()
{
	Event eventToNotify{ dae::EventType::LoadNextRound, std::tuple<bool>(false), -1 };

	dae::EventManager::GetInstance().PushEvent(eventToNotify);

}
