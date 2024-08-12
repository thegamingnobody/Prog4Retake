#include "HUDComponent.h"
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <GameObject.h>
#include <EventManager.h>
#include <TextComponent.h>


dae::HUDComponent::HUDComponent(dae::GameObject* object, dae::HUDType HUDType)
	: Component(object)
	, m_HUDType(HUDType)
	, m_StartingHealth(4)
	, m_CurrentHealth()
	, m_HealthStartPos(glm::vec3(0.0f, 0.0f, 0.0f))
{
	Reset();
	SubscribeToEvents();
}

dae::HUDComponent::~HUDComponent()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::HUDComponent::Render() const
{
	auto transformCpnt	= GetOwner()->GetComponent<dae::TransformComponent>();
	assert(transformCpnt != nullptr);

	switch (m_HUDType)
	{
	case dae::HUDType::Lives:
		{
			auto textureCpnt	= GetOwner()->GetComponent<dae::TextureComponent>();
			assert(textureCpnt != nullptr);

			for (int heart = 0; heart < m_CurrentHealth; heart++)
			{
				transformCpnt->SetWorldPosition(m_HealthStartPos.x, m_HealthStartPos.y + (heart * 15.0f));
				textureCpnt->Render();
			}
		}
		break;
	case dae::HUDType::GameOver:
		{
			auto textureCpnt	= GetOwner()->GetComponent<dae::TextureComponent>();
			assert(textureCpnt != nullptr);

			textureCpnt->Render();
		}
		break;
	case dae::HUDType::LevelCount:
	case dae::HUDType::RoundCount:
		{
			auto textCpnt = GetOwner()->GetComponent<dae::TextComponent>();
			assert(textCpnt != nullptr);

			textCpnt->Render();
		}
		break;
	}

}

void dae::HUDComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::PlayerDied:
		--m_CurrentHealth;
		if (m_CurrentHealth <= 0)
		{
			Event eventToNotify{ dae::EventType::GameOver, std::tuple<>(), -1};
			dae::EventManager::GetInstance().PushEvent(eventToNotify);
		}
		break;
	case dae::EventType::GameOver:
		{
			auto textureCpnt = GetOwner()->GetComponent<dae::TextureComponent>();	
			assert(textureCpnt != nullptr);

			textureCpnt->ToggleRender();
		}
		break;
	case dae::EventType::StartRound:
		{
			auto textCpnt = GetOwner()->GetComponent<dae::TextComponent>();	
			auto transformCpnt = GetOwner()->GetComponent<dae::TransformComponent>();
			auto arguments = event.GetArgumentsAsTuple<int, int>();

			assert(textCpnt != nullptr);
			assert(transformCpnt != nullptr);

			switch (m_HUDType)
			{
			case dae::HUDType::LevelCount:
				textCpnt->SetText(("Level: " + std::to_string(std::get<0>(arguments))));
				break;
			case dae::HUDType::RoundCount:
				textCpnt->SetText(("Round: " + std::to_string(std::get<1>(arguments))));
				break;
			default:
				break;
			}
		}
		break;
	}
}

void dae::HUDComponent::Reset()
{

	switch (m_HUDType)
	{
	case dae::HUDType::Lives:
		m_HealthStartPos = GetOwner()->GetComponent<dae::TransformComponent>()->GetPosition().GetPosition();
		m_CurrentHealth = m_StartingHealth;
		break;
	case dae::HUDType::GameOver:
		{
			auto textureCpnt = GetOwner()->GetComponent<dae::TextureComponent>();
			assert(textureCpnt != nullptr);

			textureCpnt->ToggleRender(false);
		}
		break;
	case dae::HUDType::LevelCount:
		{
			auto textCpnt = GetOwner()->GetComponent<dae::TextComponent>();	
			assert(textCpnt != nullptr);
	
			textCpnt->SetText(("Level: " + std::to_string(1)));
		}
		break;
	case dae::HUDType::RoundCount:
		{
			auto textCpnt = GetOwner()->GetComponent<dae::TextComponent>();	
			assert(textCpnt != nullptr);
	
			textCpnt->SetText(("Round: " + std::to_string(1)));
		}
		break;
	default:
		break;
	}
}

void dae::HUDComponent::SubscribeToEvents()
{
	switch (m_HUDType)
	{
	case dae::HUDType::Lives:
		dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerDied);
		break;
	case dae::HUDType::GameOver:
		dae::EventManager::GetInstance().AddObserver(this, dae::EventType::GameOver);
		break;
	case dae::HUDType::LevelCount:
	case dae::HUDType::RoundCount:
		dae::EventManager::GetInstance().AddObserver(this, dae::EventType::StartRound);
		break;
	}
}
