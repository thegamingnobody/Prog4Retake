#include "Component.h"

dae::Component::Component(GameObject* ownerObject)
	: m_pOwnerObject(ownerObject)
{
}

void dae::Component::Update(float const)
{
}

void dae::Component::Render() const
{
}

void dae::Component::SetShouldRemove()
{
	m_ShouldRemove = true;
}


dae::GameObject* dae::Component::GetOwner() const
{
	return m_pOwnerObject;
}
