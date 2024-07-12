#pragma once
#include "Component.h"
#include "Transform.h"
#include <glm/ext/vector_float3.hpp>
#include "Observer.h"

namespace dae
{
	class TransformComponent : public Component, public Observer
	{
	public:
		TransformComponent(dae::GameObject* object, float x = 0, float y = 0);


		const Transform& GetPosition(); //calculates current world pos
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& newPos);
		void SetLocalPosition(const Transform& newPos);

		void SetWorldPosition(float x, float y);
		void SetWorldPosition(const glm::vec3& newPos);
		void SetWorldPosition(const Transform& newPos);

		void Move(float x, float y);
		void Move(const glm::vec3& newPos);
		void Move(const Transform& newPos);

		void Notify(const Event& event) override;

	private:
		bool m_ShouldUpdate{ true };
		Transform m_LocalPosition;
		Transform m_WorldPosition;

		void UpdateTransform();
	};
}