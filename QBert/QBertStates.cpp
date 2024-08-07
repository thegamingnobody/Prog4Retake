#include "QBertStates.h"
#include "GameObject.h"
#include "QbertComponent.h"
#include <TransformComponent.h>
#include <EventManager.h>

dae::JumpingState::JumpingState(GameObject* object, const glm::vec3& direction, float jumpTime)
	: State(object)
	, m_MaxJumpTime(jumpTime)
	, m_AccumulatedTime(0.0f)
	, m_Direction(direction)
{
}

void dae::JumpingState::OnEnter()
{
	std::cout << "Enter Jump\n";
}
void dae::JumpingState::Update(float const deltaTime)
{
	std::cout << "Jumping\n";
	
	m_AccumulatedTime += deltaTime;

	std::tuple<glm::vec3, bool> eventArguments{ (m_Direction / m_MaxJumpTime) * deltaTime, false };

	Event eventToNotify{ dae::EventType::MoveObject, eventArguments, -1 };
	auto object{ GetObject() };
	object->NotifyComponents(eventToNotify);

	if (m_AccumulatedTime >= m_MaxJumpTime)
	{
		std::tuple<dae::TileCoordinates> eventArguments2{ dae::TileCoordinates(static_cast<int>(m_Direction.x), static_cast<int>(m_Direction.y)) };
		Event eventToNotify2{ dae::EventType::MoveFinished, eventArguments2, -1 };
		dae::EventManager::GetInstance().PushEvent(eventToNotify2);
		GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<FinishMovementState>(GetObject()));
	}
}
void dae::JumpingState::OnExit()
{
	std::cout << "Exit Jump\n";
}

dae::IdleState::IdleState(GameObject* object)
	: State(object)
{
}

void dae::IdleState::OnEnter()
{
	std::cout << "Enter Idle\n";

	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RequestMovement);
}
void dae::IdleState::Update(float const)
{
	std::cout << "Idle\n";
}
void dae::IdleState::OnExit()
{
	std::cout << "Exit Idle\n";
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
	std::cout << "Enter Request\n";
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
}
void dae::RequestingMovementState::Update(float const /*deltaTime*/)
{
	std::cout << "Request\n";
}
void dae::RequestingMovementState::OnExit()
{
	std::cout << "Exit Request\n";
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::RequestingMovementState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::ConfirmMovement:
		{
			auto arguments{ event.GetArgumentsAsTuple<bool, glm::vec3, dae::TileCoordinates>() };
			//bool isTileValid{ std::get<0>(arguments) };
			auto& newDirection{ std::get<1>(arguments) };
			//auto& originalDirection{ std::get<2>(arguments) };

			float maxJumpTime{ 0.4f };

			GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<JumpingState>(GetObject(), newDirection, maxJumpTime));
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
	std::cout << "Enter Finish\n";
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveFinished);
}
void dae::FinishMovementState::Update(float const /*deltaTime*/)
{
	std::cout << "Finish\n";
}
void dae::FinishMovementState::OnExit()
{
	std::cout << "Exit Finish\n";
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::FinishMovementState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::MoveFinished:
		{
		auto coords = GetObject()->GetComponent<dae::QbertComponent>()->GetCoords();

		std::tuple<TileCoordinates, TileCoordinates> eventArguments{ coords, std::get<0>(event.GetArgumentsAsTuple<dae::TileCoordinates>()) };

		Event eventToNotify{ dae::EventType::ToggleTile, eventArguments, -1 };

		dae::EventManager::GetInstance().PushEvent(eventToNotify);

		GetObject()->GetComponent<dae::QbertComponent>()->SetState(std::make_unique<IdleState>(GetObject()));
		}
		break;
	}
}
