#include "TransformComponent.h"
#include "GameObject.h"

dae::TransformComponent::TransformComponent(dae::GameObject* object, float const x, float const y)
	: Component(object)
	, m_ShouldUpdate(false)
	, m_LocalPosition(Transform(0, 0, 0))
	, m_WorldPosition(Transform(x, y, 0))
{
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
	
}
