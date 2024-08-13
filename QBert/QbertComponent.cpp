#include "QbertComponent.h"
#include <GameObject.h>
#include <EventManager.h>
#include <glm/ext/vector_float3.hpp>
#include <TransformComponent.h>
#include <ServiceLocator.h>
#include <DAE_SDL_SoundSystem.h>

dae::QbertComponent::QbertComponent(GameObject* object, GameObject* curseObject, int startColumn, int startRow)
	: Component(object)
	, m_Coordinates(startColumn, startRow)
	, m_CurseObject(curseObject)
	, m_Died(false)
{
	m_TargetNumber = object->GetObjectID();


	SetState(std::make_unique<dae::NullState>(GetOwner()));
	SetState(std::make_unique<dae::IdleState>(GetOwner()));

	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RespawnPlayer);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::GameOver);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerCoilyCollision);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerMoveFinished);
}

void dae::QbertComponent::Update(float const deltaTime)
{
	m_PlayerState->Update(deltaTime);
}

dae::QbertComponent::~QbertComponent()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::QbertComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::ConfirmMovement:
	{
		auto arguments{ event.GetArgumentsAsTuple<bool, glm::vec3, dae::TileCoordinates>() };
		auto& originalDirection{ std::get<2>(arguments) };
		//bool isTileValid{ std::get<0>(arguments) };

		m_Coordinates.m_Row += originalDirection.m_Row;
		m_Coordinates.m_Column += originalDirection.m_Column;

		break;
	}
	case dae::EventType::RespawnPlayer:
		{
			if (m_Coordinates.m_Row != 0 and m_Coordinates.m_Column != 0)
			{
				glm::vec3 coords{ static_cast<int>(m_Coordinates.m_Row), static_cast<int>(m_Coordinates.m_Column), 0.0f };

				//bad magic numbers and copied code from level component
				coords = glm::vec3(-(coords.x * 0.50f) + (coords.y * 0.50f), (coords.x * 0.75f) + (coords.y * 0.75f), 0);
				coords *= 32.0f * 2.0f;

				coords.x = -(coords.x);
				coords.y = -(coords.y);

				std::tuple<glm::vec3, bool, float> eventArguments{ coords, false, 0.0f };

				Event eventToNotify{ dae::EventType::MoveObject, eventArguments, -1 };
				auto owner{ GetOwner() };
				owner->NotifyComponents(eventToNotify);

			}
			m_Coordinates.m_Row = 0;
			m_Coordinates.m_Column = 0;
			m_Died = false;
		}
		break;
	case dae::EventType::PlayerCoilyCollision:
		{
			std::tuple<dae::Transform> eventArguments{ GetOwner()->GetComponent<dae::TransformComponent>()->GetPosition() };
			Event eventToNotify{ dae::EventType::PlayerDied, eventArguments, -1 };

			dae::EventManager::GetInstance().PushEvent(eventToNotify);
			m_Died = true;

		}
		break;
	case dae::EventType::PlayerMoveFinished:
		if (m_Died)
		{
			SetState(std::make_unique<DeathState>(GetOwner(), 1.0f), true);
		}
		break;
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
