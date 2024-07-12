#include "MoveCommand.h"
#include <iostream>
#include "Event.h"
#include "EventManager.h"

dae::MoveCommand::MoveCommand(int const playerNumber, glm::vec3 direction, float zoomLevel)
	: m_Direction(direction)
{
	m_PlayerNumber = playerNumber;
	float const TileSide{ 32.0f };
	
	m_Direction *= TileSide * zoomLevel;
	m_Direction = glm::vec3((m_Direction.x * 0.50f) - (m_Direction.y * 0.50f), (m_Direction.x * 0.75f) + (m_Direction.y * 0.75f), 0);
}

void dae::MoveCommand::Execute()
{
	std::tuple<const glm::vec3&> eventArguments{ m_Direction };

	Event eventToNotify{ dae::EventType::MoveObject, eventArguments, m_PlayerNumber };

	dae::EventManager::GetInstance().PushEvent(eventToNotify);
}
