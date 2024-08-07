#include "QBertStates.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include <TransformComponent.h>
#include <EventManager.h>

dae::JumpingState::JumpingState(GameObject* object, const glm::vec3& direction, float jumpTime, bool isTileValid)
	: State(object)
	, m_MaxJumpTime(jumpTime)
	, m_AccumulatedTime(0.0f)
	, m_Direction(direction)
	, m_IsTileValid(isTileValid)
{
}

void dae::JumpingState::OnEnter()
{
}
void dae::JumpingState::Update(float const deltaTime)
{	
	m_AccumulatedTime += deltaTime;

	std::tuple<glm::vec3, bool> eventArguments{ (m_Direction / m_MaxJumpTime) * deltaTime, false };

	Event eventToNotify{ dae::EventType::MoveObject, eventArguments, -1 };
	auto object{ GetObject() };
	object->NotifyComponents(eventToNotify);

	if (m_AccumulatedTime >= m_MaxJumpTime)
	{
		std::tuple<dae::TileCoordinates, bool> eventArguments2{ dae::TileCoordinates(static_cast<int>(m_Direction.x), static_cast<int>(m_Direction.y)), m_IsTileValid };
		Event eventToNotify2{ dae::EventType::MoveFinished, eventArguments2, -1 };
		dae::EventManager::GetInstance().PushEvent(eventToNotify2);
		GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<FinishMovementState>(GetObject()));
	}
}
void dae::JumpingState::OnExit()
{
}

dae::IdleState::IdleState(GameObject* object)
	: State(object)
{
}

void dae::IdleState::OnEnter()
{
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RequestMovement);
}
void dae::IdleState::Update(float const)
{
}
void dae::IdleState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::IdleState::Notify(const Event& event)
{	
	switch (event.m_type)
	{
	case dae::EventType::RequestMovement:
		{
		auto& arguments{ std::get<0>(event.GetArgumentsAsTuple<const glm::vec3&>()) };
		
		TileCoordinates TilesDirection{static_cast<int>(arguments.x), static_cast<int>(arguments.y)};

		auto coords = GetObject()->GetComponent<dae::QbertComponent>()->GetCoords();

		std::tuple<TileCoordinates, TileCoordinates> eventArguments{ coords, TilesDirection };

		Event eventToNotify{ dae::EventType::IsTileValid, eventArguments, -1 };

		dae::EventManager::GetInstance().PushEvent(eventToNotify);

		GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<RequestingMovementState>(GetObject()));
		}
		break;
	}

}

dae::RequestingMovementState::RequestingMovementState(GameObject* object)
	: State(object)
{
}

void dae::RequestingMovementState::OnEnter()
{
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
}
void dae::RequestingMovementState::Update(float const /*deltaTime*/)
{
}
void dae::RequestingMovementState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::RequestingMovementState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::ConfirmMovement:
		{
			auto arguments{ event.GetArgumentsAsTuple<bool, glm::vec3, dae::TileCoordinates>() };
			bool isTileValid{ std::get<0>(arguments) };
			auto& newDirection{ std::get<1>(arguments) };
			//auto& originalDirection{ std::get<2>(arguments) };

			float maxJumpTime{ 0.4f };

			GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<JumpingState>(GetObject(), newDirection, maxJumpTime, isTileValid));

		}
		break;
	}
}

dae::FinishMovementState::FinishMovementState(GameObject* object)
	: State(object)
{
}

void dae::FinishMovementState::OnEnter()
{
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveFinished);
}
void dae::FinishMovementState::Update(float const /*deltaTime*/)
{
}
void dae::FinishMovementState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::FinishMovementState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::MoveFinished:
		{
			auto coords = GetObject()->GetComponent<dae::QbertComponent>()->GetCoords();

			auto arguments = event.GetArgumentsAsTuple<dae::TileCoordinates, bool>();

			if (std::get<1>(arguments))
			{
				std::tuple<TileCoordinates, TileCoordinates> eventArguments{ coords, std::get<0>(arguments) };
				Event eventToNotify{ dae::EventType::ToggleTile, eventArguments, -1 };
				dae::EventManager::GetInstance().PushEvent(eventToNotify);
				GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<IdleState>(GetObject()));
				break;
			}
			else
			{
				auto transformCpnt = GetObject()->GetComponent<dae::TransformComponent>();
				if (transformCpnt)
				{
					std::tuple<dae::Transform> eventArguments{ transformCpnt->GetPosition() };

					Event eventToNotify{ dae::EventType::PlayerDied, eventArguments, -1 };

					dae::EventManager::GetInstance().PushEvent(eventToNotify);
					GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<DeathState>(GetObject(), 1.0f));
					break;
				}
			}
		}
		break;
	}
}

dae::DeathState::DeathState(GameObject* object, float deathTimer)
	: State(object)
	, m_DeathTimer(deathTimer)
	, m_AccumulatedTime(0.0f)
{

}

void dae::DeathState::OnEnter()
{
}
void dae::DeathState::Update(float const deltaTime)
{
	m_AccumulatedTime += deltaTime;

	if (m_AccumulatedTime >= m_DeathTimer)
	{
		Event eventToNotify{ dae::EventType::RespawnPlayer, std::tuple<>(), -1};

		dae::EventManager::GetInstance().PushEvent(eventToNotify);

		GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<IdleState>(GetObject()));
	}
}
void dae::DeathState::OnExit()
{
}
