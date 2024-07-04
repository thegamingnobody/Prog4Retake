#pragma once
#include <memory>
#include <string>
#include <vector>
#include "Component.h"

namespace dae
{
	class Texture2D;

	class GameObject final
	{
	public:
		void Update(float const deltaTime);
		void Render() const;

		void SetParent(GameObject* newParent, bool keepPosition);
		GameObject* GetParent() const;

		template<class CpntType, class... Arguments>
		requires std::derived_from<CpntType, Component>
		CpntType& AddComponent(Arguments&&... arguments)
		{
			auto createComponent = [this](Arguments&&... arguments) {
				auto component = std::make_shared<CpntType>(this, std::forward<Arguments>(arguments)...);
				m_pComponents.emplace_back(component);
				return component;
				};

			return *createComponent(std::forward<Arguments>(arguments)...);
		}

		template<class CpntType>
		requires std::derived_from<CpntType, Component>
		CpntType* GetComponent() const
		{
			if (m_pComponents.size() == 0)
				return nullptr;

			for (auto& component : m_pComponents)
			{
				if (auto derivedComponent = dynamic_cast<CpntType*>(component.get()))
				{
					return derivedComponent;
				}
			}

			return nullptr;
		}

		template<class CpntType>
		requires std::derived_from<CpntType, Component>
		bool HasComponent() const
		{
			CpntType* component{ GetComponent<CpntType>() };
			if (component == nullptr)
			{
				return false;
			}
			
			return true;
		}

		GameObject(const std::string& ObjectName = "");
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

	private:
		std::string const m_ObjectName;

		GameObject* m_pOwnerObject{ nullptr };
		std::vector<GameObject*> m_pChildObjects{};

		std::vector<std::shared_ptr<Component>> m_pComponents{};

	};
}
