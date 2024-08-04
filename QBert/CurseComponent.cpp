#include "CurseComponent.h"
#include <GameObject.h>
#include <EventManager.h>
#include <TransformComponent.h>
#include <TextureComponent.h>

dae::CurseComponent::CurseComponent(GameObject* object)
	: Component(object)
{
	m_TargetNumber = object->GetObjectID();
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerDied);

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

		owner->GetComponent<dae::TextureComponent>()->ToggleRender();
		break;
	}
	}
}
