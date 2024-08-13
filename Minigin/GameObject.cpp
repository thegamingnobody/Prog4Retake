#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "TransformComponent.h"
#include "TextureComponent.h"


dae::GameObject::GameObject(const std::string& ObjectName, int const playerNumber)
	: m_ObjectName(ObjectName)
	, m_pOwnerObject( nullptr )
	, m_pChildObjects()
	, m_pComponents()
	, m_PlayerNumber(playerNumber)
	, m_ShouldRemove(false)
{
}

dae::GameObject::~GameObject() = default;

void dae::GameObject::NotifyComponents(const Event& event)
{
	for (auto& component : m_pComponents)
	{
		if (auto derivedComponent = dynamic_cast<Observer*>(component.get()))
		{
			derivedComponent->Notify(event);
		}
	}
}

void dae::GameObject::Update(float const deltaTime)
{
	for (auto& component : m_pComponents)
	{
		component->Update(deltaTime);
	}

}

void dae::GameObject::Render() const
{
	for (auto& component : m_pComponents)
	{
		component->Render();
	}
}

void dae::GameObject::SetParent(GameObject* newParent, bool keepPosition)
{
	//check if new parent is valid
	if (newParent == nullptr or newParent == this) return;

	if (keepPosition)
	{
		auto transformCpnt{ GetComponent<dae::TransformComponent>() };
		auto ownerTransformCpnt{ m_pOwnerObject->GetComponent<dae::TransformComponent>() };

		if (transformCpnt != nullptr and ownerTransformCpnt != nullptr)
		{
			transformCpnt->SetLocalPosition(transformCpnt->GetPosition() - ownerTransformCpnt->GetPosition());

		}

	}

	//remove child from old parent
	if (m_pOwnerObject)
	{
		auto it = std::find(m_pOwnerObject->m_pChildObjects.begin(), m_pOwnerObject->m_pChildObjects.end(), this);

		if (it != m_pOwnerObject->m_pChildObjects.end())
		{
			m_pOwnerObject->m_pChildObjects.erase(it);
		}
	}

	//set new parent
	m_pOwnerObject = newParent;

	//add to parent as child
	m_pOwnerObject->m_pChildObjects.emplace_back(this);

	//auto transformThis{ GetComponent<Transform>() };
	//auto transformOwner{ m_pOwnerObject->GetComponent<Transform>() };

	//if (transformThis.has_value() and transformOwner.has_value())
	//{
	//	transformThis.value()->SetShouldUpdate(true);
	//}

}

dae::GameObject* dae::GameObject::GetParent() const
{
	return m_pOwnerObject;
}
