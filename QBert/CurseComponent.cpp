#include "CurseComponent.h"
#include <GameObject.h>
#include <EventManager.h>
#include <TransformComponent.h>
#include <TextureComponent.h>
#include <ServiceLocator.h>
#include "SFXEnum.h"

dae::CurseComponent::CurseComponent(GameObject* object)
	: Component(object)
{
	m_TargetNumber = object->GetObjectID();
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerDied);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::RespawnPlayer);

}

dae::CurseComponent::~CurseComponent()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::CurseComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::PlayerDied:
	{
		auto owner{ GetOwner() };
		if (not owner) break;

		auto arguments{ event.GetArgumentsAsTuple<dae::Transform>() };
		auto pos{ std::get<0>(arguments) };

		pos.SetPosition(pos.GetPosition().x, pos.GetPosition().y - 32, pos.GetPosition().z);

		auto transformComponent = owner->GetComponent<dae::TransformComponent>();
		if (not transformComponent) break;

		transformComponent->MoveTo(pos);

		owner->GetComponent<dae::TextureComponent>()->ToggleRender(true);

		dae::ServiceLocator::GetSoundSystem().PlaySound(SoundId(dae::SFX::Swearing));
		break;
	}
	case dae::EventType::RespawnPlayer:
		{
			auto owner{ GetOwner() };
			if (not owner) break;

			owner->GetComponent<dae::TextureComponent>()->ToggleRender(false);
		}
		break;
	}
}
