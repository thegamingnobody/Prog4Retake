#include "TransformComponent.h"
#include "GameObject.h"
#include "EventManager.h"

dae::TransformComponent::TransformComponent(dae::GameObject* object, float const x, float const y)
	: Component(object)
	, m_ShouldUpdate(false)
	, m_LocalPosition(Transform(0, 0, 0))
	, m_WorldPosition(Transform(x, y, 0))
{
	m_TargetNumber = object->GetObjectID();
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveObject);
}

const dae::Transform& dae::TransformComponent::GetPosition()
{
	UpdateTransform();
	return m_WorldPosition;
}

void dae::TransformComponent::SetLocalPosition(float const x, float const y)
{
	m_LocalPosition.SetPosition(x, y, 0);
}

void dae::TransformComponent::SetLocalPosition(const glm::vec3& newPos)
{
	SetLocalPosition(newPos.x, newPos.y);
}

void dae::TransformComponent::SetLocalPosition(const Transform& newPos)
{
	SetLocalPosition(newPos.GetPosition().x, newPos.GetPosition().y);
}

void dae::TransformComponent::SetWorldPosition(float const x, float const y)
{
	m_WorldPosition.SetPosition(x, y, 0);
}

void dae::TransformComponent::SetWorldPosition(const glm::vec3& newPos)
{
	SetWorldPosition(newPos.x, newPos.y);
}

void dae::TransformComponent::SetWorldPosition(const Transform& newPos)
{
	SetWorldPosition(newPos.GetPosition().x, newPos.GetPosition().y);
}

void dae::TransformComponent::Move(float x, float y)
{
	m_LocalPosition.SetPosition(m_LocalPosition.GetPosition().x + x, m_LocalPosition.GetPosition().y + y, 0);
}

void dae::TransformComponent::Move(const glm::vec3& newPos)
{
	Move(newPos.x, newPos.y);
}

void dae::TransformComponent::Move(const Transform& newPos)
{
	Move(newPos.GetPosition().x, newPos.GetPosition().y);
}

void dae::TransformComponent::Notify(const Event& event)
{
	switch (event.m_type)
	{
	case dae::EventType::MoveObject:
	{
		auto castedArguments{ std::get<0>(event.GetArgumentsAsTuple<const glm::vec3&>()) };
		Move(castedArguments.x, castedArguments.y);
	}
	break;
	}

}

void dae::TransformComponent::UpdateTransform()
{
	auto owner{ GetOwner() };
	auto parentOwner{ owner->GetParent() };

	if (owner == nullptr)
	{
		m_WorldPosition.SetPosition(m_LocalPosition);
	}
	else if (not(parentOwner == nullptr))
	{
		auto transformCpntParentOfOwner{ parentOwner->GetComponent<TransformComponent>() };
		if (transformCpntParentOfOwner)
		{
			m_WorldPosition = transformCpntParentOfOwner->GetPosition() + m_LocalPosition;
		}
	}
	else
	{
		m_WorldPosition = m_WorldPosition + m_LocalPosition;
		m_LocalPosition.SetPosition(0.0f, 0.0f, 0.0f);
	}
	
}
