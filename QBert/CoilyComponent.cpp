#include "CoilyComponent.h"
#include "GameObject.h"
#include <EventManager.h>

dae::CoilyComponent::CoilyComponent(GameObject* object, dae::QbertComponent& playerComponent, int startColumn, int startRow)
	: Component(object)
	, m_Coordinates(startColumn, startRow)
	, m_CoilyForm(dae::CoilyComponent::CoilyForm::Egg)
	, m_PlayerComponent(playerComponent)
	, m_PlayerCoordinates(0, 0)
{
	m_TargetNumber = object->GetObjectID();

	SetState(std::make_unique<dae::NullState>(GetOwner()));
	SetState(std::make_unique<dae::CoilyIdleState>(GetOwner()));

	m_PlayerCoordinates = m_PlayerComponent.GetCoords();

	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::ConfirmMovement);
	//dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RespawnPlayer);
	//dae::EventManager::GetInstance().AddObserver(this, dae::EventType::GameOver);
}

void dae::CoilyComponent::Update(float const deltaTime)
{
	m_PlayerCoordinates = m_PlayerComponent.GetCoords();
	m_CoilyState->Update(deltaTime);
}

dae::CoilyComponent::~CoilyComponent()
{
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
			std::cout << "New coords coily: \n   row: " << m_Coordinates.m_Row << "\ncolumn: " << m_Coordinates.m_Column << "\n";
		}
		break;
	}
	case dae::EventType::RespawnPlayer:

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

		m_Coordinates.m_Row = 0;
		m_Coordinates.m_Column = 0;
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
