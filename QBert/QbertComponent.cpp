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
{
	m_TargetNumber = object->GetObjectID();

	SetState(std::make_unique<dae::IdleState>(GetOwner()));

	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RespawnPlayer);
}

void dae::QbertComponent::Update(float const deltaTime)
{
	m_PlayerState->Update(deltaTime);
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

		glm::vec3 coords{ static_cast<int>(m_Coordinates.m_Row), static_cast<int>(m_Coordinates.m_Column), 0.0f };

		coords.x = -(coords.x);
		coords.y = -(coords.y);

		//bad magic numbers and copied code from level component
		coords = glm::vec3(((coords.x * 0.50f) + (coords.y * 0.50f)), ((coords.x * 0.75f) + (coords.y * 0.75f)), 0);
		coords *= 32.0f * 2.0f;


		std::tuple<glm::vec3, bool> eventArguments{ coords, false };

		Event eventToNotify{ dae::EventType::MoveObject, eventArguments, -1 };
		auto owner{ GetOwner() };
		owner->NotifyComponents(eventToNotify);

		//m_Coordinates.m_Row = 0;
		//m_Coordinates.m_Column = 0;


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
