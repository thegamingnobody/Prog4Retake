#include "TransformComponent.h"
#include "GameObject.h"
#include "EventManager.h"
#include "Level.h"

dae::TransformComponent::TransformComponent(dae::GameObject* object, float const x, float const y)
	: Component(object)
	, m_ShouldUpdate(false)
	, m_LocalPosition(Transform(0, 0, 0))
	, m_WorldPosition(Transform(x, y, 0))
	, m_Direction(Transform(0, 0, 0))
	, m_AccumulatedTime(0.0f)
	, m_IsMoving()
{
	m_TargetNumber = object->GetObjectID();
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveObject);
	dae::EventManager::GetInstance().AddObserver(this, dae::EventType::MoveObjectTo);
}

dae::TransformComponent::~TransformComponent()
{
	dae::EventManager::GetInstance().RemoveObserver(this);
	dae::EventManager::GetInstance().RemoveObserver(this);
}

void dae::TransformComponent::Update(float const deltaTime)
{
	if (m_IsMoving)
	{
		m_AccumulatedTime += deltaTime;

		if (m_AccumulatedTime <= m_SlideTime)
		{
			Move((m_Direction.GetPosition() / m_SlideTime) * deltaTime);
		}
		else
		{
			auto arguments = std::make_tuple<dae::TileCoordinates>(dae::TileCoordinates(static_cast<int>(m_Direction.GetPosition().y), static_cast<int>(m_Direction.GetPosition().x)));

			Event eventToNotify{ dae::EventType::MoveFinished, arguments, m_TargetNumber };

			GetOwner()->NotifyComponents(eventToNotify);
		
			m_IsMoving = false;
		}
	}
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

void dae::TransformComponent::MoveTo(float x, float y)
{
	Move(x - m_WorldPosition.GetPosition().x, y - m_WorldPosition.GetPosition().y);
}
void dae::TransformComponent::MoveTo(const glm::vec3& newPos)
{
	MoveTo(newPos.x, newPos.y);
}
void dae::TransformComponent::MoveTo(const Transform& newPos)
{
	MoveTo(newPos.GetPosition().x, newPos.GetPosition().y);
}

void dae::TransformComponent::Notify(const Event& event)
{

	switch (event.m_type)
	{
	case dae::EventType::MoveObject:
	{
		auto castedArguments{ event.GetArgumentsAsTuple<glm::vec3, bool, float>() };
		auto newDir		{ std::get<0>(castedArguments) };
		auto shouldSlide{ std::get<1>(castedArguments) };
		m_SlideTime = std::get<2>(castedArguments);

		if (not(shouldSlide))
		{
			m_AccumulatedTime = 10000.0f;
			m_IsMoving = true;
			Move(newDir);
		}
		else
		{
			m_Direction = newDir;
			m_AccumulatedTime = 0.0f;
			m_IsMoving = true;
		}
	}
	break;
	case dae::EventType::MoveObjectTo:
	{
		auto castedArguments{ event.GetArgumentsAsTuple<glm::vec3, bool, float>() };
		auto newDir{ std::get<0>(castedArguments) };
		auto shouldSlide{ std::get<1>(castedArguments) };
		m_SlideTime = std::get<2>(castedArguments);

		if (not(shouldSlide))
		{
			MoveTo(newDir);
		}
		else
		{
			m_Direction = newDir;
			m_AccumulatedTime = 0.0f;
			m_IsMoving = true;
		}
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
