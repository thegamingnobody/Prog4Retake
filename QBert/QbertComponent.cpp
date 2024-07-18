#include "QbertComponent.h"
#include <GameObject.h>
#include <EventManager.h>
#include <glm/ext/vector_float3.hpp>

dae::QbertComponent::QbertComponent(GameObject* object, int startColumn, int startRow)
	: Component(object)
	, m_Coordinates(startColumn, startRow)
	, m_TilesDirection(startColumn, startRow)
	, m_DirectionSet(false)
{
	m_TargetNumber = object->GetObjectID();
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RequestMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
}

void dae::QbertComponent::Update(float const)
{
	m_DirectionSet = false;
}

void dae::QbertComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::RequestMovement:
		if (not(m_DirectionSet))
		{
			auto& arguments{ std::get<0>(event.GetArgumentsAsTuple<const glm::vec3&>()) };
			m_TilesDirection.m_Column = static_cast<int>(arguments.x);
			m_TilesDirection.m_Row = static_cast<int>(arguments.y);

			m_DirectionSet = true;

			std::tuple<TileCoordinates, TileCoordinates> eventArguments{ m_Coordinates, m_TilesDirection };

			Event eventToNotify{ dae::EventType::IsTileValid, eventArguments, -1 };

			dae::EventManager::GetInstance().PushEvent(eventToNotify);
		}

		break;
	case dae::EventType::ConfirmMovement:
		{
			auto arguments{ event.GetArgumentsAsTuple<bool, glm::vec3>() };
			bool isTileValid{ std::get<0>(arguments) };
			auto& newDirection{ std::get<1>(arguments) };

			if (isTileValid)
			{
				m_Coordinates.m_Column += m_TilesDirection.m_Column;
				m_Coordinates.m_Row += m_TilesDirection.m_Row;
				std::tuple<glm::vec3> eventArguments{ newDirection };

				Event eventToNotify{ dae::EventType::MoveObject, eventArguments, -1 };
				GetOwner()->NotifyComponents(eventToNotify);
			}
		break;
		}
	}
}
