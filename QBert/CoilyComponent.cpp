#include "CoilyComponent.h"
#include "GameObject.h"
#include <EventManager.h>
#include <TransformComponent.h>
#include <SceneManager.h>
#include <Scene.h>

dae::CoilyComponent::CoilyComponent(GameObject* object, dae::QbertComponent& playerComponent, int startColumn, int startRow)
	: Component(object)
	, m_Coordinates(startColumn, startRow)
	, m_CoilyForm(dae::CoilyComponent::CoilyForm::Egg)
	, m_PlayerComponent(playerComponent)
	, m_PlayerCoordinates(0, 0)
	, m_CollisionEventSent(false)
{
	m_TargetNumber = object->GetObjectID();

	SetState(std::make_unique<dae::NullState>(GetOwner()));
	SetState(std::make_unique<dae::CoilyIdleState>(GetOwner()));

	m_PlayerCoordinates = m_PlayerComponent.GetCoords();

	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerDied);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RespawnPlayer);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::GameOver);
}

void dae::CoilyComponent::Update(float const deltaTime)
{
	m_PlayerCoordinates = m_PlayerComponent.GetCoords();
	m_CoilyState->Update(deltaTime);

	if (m_PlayerCoordinates == m_Coordinates)
	{
		if (not m_CollisionEventSent)
		{
			Event eventToNotify{ dae::EventType::PlayerCoilyCollision, std::tuple<>(), -1 };
			dae::EventManager::GetInstance().PushEvent(eventToNotify);
			m_CollisionEventSent = true;
		}
		else
		{
			m_CollisionEventSent = false;
		}

	}
}

dae::CoilyComponent::~CoilyComponent()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::CoilyComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
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
		break;
	}
	case dae::EventType::PlayerDied:
	case dae::EventType::RespawnPlayer:
	case dae::EventType::GameOver:
		GetOwner()->Remove();
		break;
	}
}

void dae::CoilyComponent::SetState(std::unique_ptr<State>&& newState, bool callOnExit)
{
	if (callOnExit and m_CoilyState)
	{
		m_CoilyState->OnExit();
	}

	if (newState)
	{
		m_CoilyState = std::move(newState);
		m_CoilyState->OnEnter();
	}
}

void dae::CoilyComponent::HatchIntoCoily()
{
	m_CoilyForm = CoilyForm::Snake;

	auto textureCpnt = GetOwner()->GetComponent<TextureComponent>();
	assert(textureCpnt != nullptr);

	auto srcRect = textureCpnt->GetSourceRectangle(); 

	srcRect.SrcPosX = 144.0f;

	textureCpnt->SetSourceRectangle(srcRect);
}
