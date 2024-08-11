#include "HUDComponent.h"
#include <TextureComponent.h>
#include <TransformComponent.h>
#include <GameObject.h>
#include <EventManager.h>


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

void dae::HUDComponent::Render() const
{
	auto textureCpnt   = GetOwner()->GetComponent<dae::TextureComponent>();
	auto transformCpnt = GetOwner()->GetComponent<dae::TransformComponent>();

	assert(textureCpnt != nullptr);
	assert(transformCpnt != nullptr);

	switch (m_HUDType)
	{
	case dae::HUDType::Lives:
		for (int heart = 0; heart < m_CurrentHealth; heart++)
		{
			transformCpnt->SetWorldPosition(m_HealthStartPos.x, m_HealthStartPos.y + (heart * 15.0f));
			textureCpnt->Render();
		}
		break;
	case dae::HUDType::GameOver:
		textureCpnt->Render();
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
	}
}

void dae::HUDComponent::Reset()
{
	m_HealthStartPos = GetOwner()->GetComponent<dae::TransformComponent>()->GetPosition().GetPosition();
	m_CurrentHealth = m_StartingHealth;
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
	}
}
