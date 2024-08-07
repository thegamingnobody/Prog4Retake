#include "QbertComponent.h"
#include <GameObject.h>
#include <EventManager.h>
#include <glm/ext/vector_float3.hpp>
#include <TransformComponent.h>

dae::QbertComponent::QbertComponent(GameObject* object, GameObject* curseObject, int startColumn, int startRow)
	: Component(object)
	, m_Coordinates(startColumn, startRow)
	//, m_TilesDirection(startColumn, startRow)
	//, m_DirectionSet(false)
	//, m_MovementState(dae::QbertComponent::MovementState::Idle)
	, m_CurseObject(curseObject)
{
	m_TargetNumber = object->GetObjectID();

	SetState(std::make_unique<dae::IdleState>(GetOwner()));

	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RespawnPlayer);

	//std::tuple<const glm::vec3&> eventArguments{ glm::vec3(0.0f, 0.0f, 0.0f) };
	//Event eventToNotify{ dae::EventType::RequestMovement, eventArguments, m_TargetNumber };
	//dae::EventManager::GetInstance().PushEvent(eventToNotify);

}

void dae::QbertComponent::Update(float const deltaTime)
{
	m_PlayerState->Update(deltaTime);
}

void dae::QbertComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	//case dae::EventType::RequestMovement:
		//if (decltype(m_PlayerState) == decltype(dae::IdleState))
		//{
		//	SetState(std::make_unique<dae::JumpingState>(GetOwner()));
		//}

	//	break;
	case dae::EventType::ConfirmMovement:
		{
			auto arguments{ event.GetArgumentsAsTuple<bool, glm::vec3, dae::TileCoordinates>() };
			auto& originalDirection{ std::get<2>(arguments) };
			bool isTileValid{ std::get<0>(arguments) };

			if (isTileValid)
			{
				m_Coordinates.m_Row += originalDirection.m_Row;
				m_Coordinates.m_Column += originalDirection.m_Column;
			}

			//if (not(isTileValid))
			//{
			//	m_MovementState = MovementState::Dead;				 
			//}
			break;
		}
	case dae::EventType::MoveFinished:
		{
			//std::tuple<TileCoordinates, TileCoordinates> eventArguments{ m_Coordinates, m_TilesDirection };

			//Event eventToNotify{ dae::EventType::ToggleTile, eventArguments, -1 };

			//dae::EventManager::GetInstance().PushEvent(eventToNotify);

			//if (not(m_MovementState == MovementState::Dead))
			//{
			//	m_MovementState = MovementState::Idle;
			//	SetState(std::make_unique<dae::IdleState>(GetOwner()));
			//}
			//else
			//{
			//	if (not m_CurseObject) return;

			//	auto playerpos{ GetOwner()->GetComponent<dae::TransformComponent>() };

			//	Event newEventToNotify = Event(dae::EventType::PlayerDied, std::tuple<dae::Transform>(playerpos->GetPosition()), m_CurseObject->GetObjectID());
			//	dae::EventManager::GetInstance().PushEvent(newEventToNotify);
			//}
			//break;
		}
	}
}

void dae::QbertComponent::SetState(std::unique_ptr<State>&& newState, bool callOnExit)
{
	if (callOnExit and m_PlayerState)
	{
		m_PlayerState->OnExit();
	}

	if (newState)
	{
		m_PlayerState = std::move(newState);
		m_PlayerState->OnEnter();
	}
}
