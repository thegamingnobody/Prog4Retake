#include "CoilyStates.h"
#include "GameObject.h"
#include "CoilyComponent.h"
#include <TransformComponent.h>
#include <EventManager.h>
#include <ServiceLocator.h>
#include "SFXEnum.h"

dae::CoilyJumpingState::CoilyJumpingState(GameObject* object, const glm::vec3& direction, float jumpTime, bool isTileValid)
	: State(object)
	, m_MaxJumpTime(jumpTime)
	, m_AccumulatedTime(0.0f)
	, m_Direction(direction)
	, m_IsTileValid(isTileValid)
{
}

void dae::CoilyJumpingState::OnEnter()
{
}
void dae::CoilyJumpingState::Update(float const deltaTime)
{
	m_AccumulatedTime += deltaTime;

	std::tuple<glm::vec3, bool, float> eventArguments{ (m_Direction / m_MaxJumpTime) * deltaTime, false, m_MaxJumpTime };

	auto object{ GetObject() };
	Event eventToNotify{ dae::EventType::MoveObject, eventArguments, object->GetObjectID() };
	object->NotifyComponents(eventToNotify);

	if (m_AccumulatedTime >= m_MaxJumpTime)
	{
		std::tuple<dae::TileCoordinates, bool> eventArguments2{ dae::TileCoordinates(static_cast<int>(m_Direction.x), static_cast<int>(m_Direction.y)), m_IsTileValid };
		Event eventToNotify2{ dae::EventType::MoveFinished, eventArguments2, object->GetObjectID() };
		dae::EventManager::GetInstance().PushEvent(eventToNotify2);
		GetObject()->GetComponent<dae::CoilyComponent>()->SetState(std::make_unique<CoilyFinishMovementState>(GetObject()));
	}
}
void dae::CoilyJumpingState::OnExit()
{
}

dae::CoilyIdleState::CoilyIdleState(GameObject* object)
	: State(object)
	, m_AccumulatedTime(0.0f)
	, m_Directions()
{
	m_TargetNumber = object->GetObjectID();
	m_Directions.emplace_back(glm::vec3( 1,  0,  0)); //right
	m_Directions.emplace_back(glm::vec3( 0,  1,  0)); //down
	m_Directions.emplace_back(glm::vec3( 0, -1,  0)); //up
	m_Directions.emplace_back(glm::vec3(-1,  0,  0)); //left
}

void dae::CoilyIdleState::OnEnter()
{
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RequestMovement);
	//dae::EventManager::GetInstance().AddObserver(this, dae::EventType::LoadNextRound);
	//dae::EventManager::GetInstance().AddObserver(this, dae::EventType::StartRound);
}
void dae::CoilyIdleState::Update(float const deltaTime)
{
	m_AccumulatedTime += deltaTime;

	if (m_AccumulatedTime >= 0.5f)
	{
		int directionID{ CalculateNextDirectionID() };

		std::tuple<const glm::vec3&> eventArguments{ m_Directions[directionID] };
		Event eventToNotify{ dae::EventType::RequestMovement, eventArguments, m_TargetNumber };
		dae::EventManager::GetInstance().PushEvent(eventToNotify);
	}
}
void dae::CoilyIdleState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
	//dae::EventManager::GetInstance().RemoveObserver(this);
	//dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::CoilyIdleState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::RequestMovement:
	{
		//if (m_AllowMovement)
		{
			auto& arguments{ std::get<0>(event.GetArgumentsAsTuple<const glm::vec3&>()) };
			TileCoordinates TilesDirection{ static_cast<int>(arguments.x), static_cast<int>(arguments.y) };
			auto coords = GetObject()->GetComponent<dae::CoilyComponent>()->GetCoords();

			std::tuple<TileCoordinates, TileCoordinates, int> eventArguments{ coords, TilesDirection, GetObject()->GetObjectID() };
			Event eventToNotify{ dae::EventType::IsTileValid, eventArguments, -1 };
			dae::EventManager::GetInstance().PushEvent(eventToNotify);

			GetObject()->GetComponent<dae::CoilyComponent>()->SetState(std::make_unique<CoilyRequestingMovementState>(GetObject()));
		}
	}
	break;
	case dae::EventType::LoadNextRound:
		//m_AllowMovement = false;
		break;
	case dae::EventType::StartRound:
		//m_AllowMovement = true;
		break;
	}

}

int dae::CoilyIdleState::CalculateNextDirectionID()
{
	auto coilyCpnt = GetObject()->GetComponent<dae::CoilyComponent>();

	switch (coilyCpnt->GetCoilyForm())
	{
	case dae::CoilyComponent::CoilyForm::Egg:
		return rand() % static_cast<int>(coilyCpnt->GetCoilyForm());
	case dae::CoilyComponent::CoilyForm::Snake:
		{
			auto playerCoords = coilyCpnt->GetPlayerCoords();
			auto coilyCoords = coilyCpnt->GetCoords();

			auto totalMovementToMake = (playerCoords - coilyCoords);
			
			if (abs(totalMovementToMake.m_Row) > abs(totalMovementToMake.m_Column))
			{
				if (totalMovementToMake.m_Row < 0)
				{
					return 2;
				}
				else
				{
					return 1;
				}
			}
			else
			{
				if (totalMovementToMake.m_Column < 0)
				{
					return 3;
				}
				else
				{
					return 0;
				}
			}

		}
		break;
	}

	return 0;
}

dae::CoilyRequestingMovementState::CoilyRequestingMovementState(GameObject* object)
	: State(object)
{
	m_TargetNumber = object->GetObjectID();
}

void dae::CoilyRequestingMovementState::OnEnter()
{
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
}
void dae::CoilyRequestingMovementState::Update(float const)
{
}
void dae::CoilyRequestingMovementState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::CoilyRequestingMovementState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::ConfirmMovement:
	{
		auto arguments{ event.GetArgumentsAsTuple<bool, glm::vec3, dae::TileCoordinates>() };
		bool isTileValid{ std::get<0>(arguments) };
		auto& newDirection{ std::get<1>(arguments) };
		auto& originalDirection{ std::get<2>(arguments) };

		if (isTileValid)
		{
			float maxJumpTime{ 0.35f };

			GetObject()->GetComponent<dae::CoilyComponent>()->SetState(std::make_unique<CoilyJumpingState>(GetObject(), newDirection, maxJumpTime, isTileValid));
		}
		else
		{
			originalDirection.m_Column = -(originalDirection.m_Column);
			originalDirection.m_Row = -(originalDirection.m_Row);

			std::tuple<bool, glm::vec3, dae::TileCoordinates> eventArguments{ isTileValid, newDirection, originalDirection };
			Event eventToNotify{ dae::EventType::IsTileValid, eventArguments, -1 };
			GetObject()->NotifyComponents(eventToNotify);

			GetObject()->GetComponent<dae::CoilyComponent>()->HatchIntoCoily();
			GetObject()->GetComponent<dae::CoilyComponent>()->SetState(std::make_unique<CoilyIdleState>(GetObject()));


		}

	}
	break;
	}
}

dae::CoilyFinishMovementState::CoilyFinishMovementState(GameObject* object)
	: State(object)
{
	m_TargetNumber = object->GetObjectID();
}

void dae::CoilyFinishMovementState::OnEnter()
{
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveFinished);
}
void dae::CoilyFinishMovementState::Update(float const)
{
}
void dae::CoilyFinishMovementState::OnExit()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::CoilyFinishMovementState::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::MoveFinished:
	{
		//play jumping sound
		dae::ServiceLocator::GetSoundSystem().PlaySound(SoundId(SFX::CoilyEggJump));

		auto coords = GetObject()->GetComponent<dae::CoilyComponent>()->GetCoords();

		auto arguments = event.GetArgumentsAsTuple<dae::TileCoordinates, bool>();

		if (std::get<1>(arguments))
		{
			GetObject()->GetComponent<dae::CoilyComponent>()->SetState(std::make_unique<CoilyIdleState>(GetObject()));
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
				//GetObject()->GetComponent<dae::CoilyComponent>()->SetState(std::make_unique<DeathState>(GetObject(), 1.0f));
				break;
			}
		}
	}
	break;
	}
}
