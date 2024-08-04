#include "QbertComponent.h"
#include <GameObject.h>
#include <EventManager.h>
#include <glm/ext/vector_float3.hpp>
#include <TransformComponent.h>

dae::QbertComponent::QbertComponent(GameObject* object, GameObject* curseObject, int startColumn, int startRow)
	: Component(object)
	, m_Coordinates(startColumn, startRow)
	, m_TilesDirection(startColumn, startRow)
	, m_DirectionSet(false)
	, m_MovementState(dae::QbertComponent::MovementState::Idle)
	, m_CurseObject(curseObject)
{
	m_TargetNumber = object->GetObjectID();
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RequestMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveFinished);

	std::tuple<const glm::vec3&> eventArguments{ glm::vec3(0.0f, 0.0f, 0.0f) };

	Event eventToNotify{ dae::EventType::RequestMovement, eventArguments, m_TargetNumber };

	dae::EventManager::GetInstance().PushEvent(eventToNotify);

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
		if (not(m_DirectionSet) and m_MovementState == MovementState::Idle)
		{
			m_MovementState = MovementState::Requesting;
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

			m_MovementState = MovementState::Moving;
			m_Coordinates.m_Column += m_TilesDirection.m_Column;
			m_Coordinates.m_Row += m_TilesDirection.m_Row;
			std::tuple<glm::vec3, bool> eventArguments{ newDirection, true };

			Event eventToNotify{ dae::EventType::MoveObject, eventArguments, -1 };
			auto owner{ GetOwner() };
			owner->NotifyComponents(eventToNotify);

			if (not(isTileValid))
			{
				m_MovementState = MovementState::Dead;				 
			}
			break;
		}
	case dae::EventType::MoveFinished:
		{
			std::tuple<TileCoordinates, TileCoordinates> eventArguments{ m_Coordinates, m_TilesDirection };

			Event eventToNotify{ dae::EventType::ToggleTile, eventArguments, -1 };

			dae::EventManager::GetInstance().PushEvent(eventToNotify);

			if (not(m_MovementState == MovementState::Dead))
			{
				m_MovementState = MovementState::Idle;
			}
			else
			{
				if (not m_CurseObject) return;

				auto playerpos{ GetOwner()->GetComponent<dae::TransformComponent>() };

				Event newEventToNotify = Event(dae::EventType::PlayerDied, std::tuple<dae::Transform>(playerpos->GetPosition()), m_CurseObject->GetObjectID());
				dae::EventManager::GetInstance().PushEvent(newEventToNotify);
			}
			break;
		}
	}
}
