#pragma once
#include "Component.h"
#include "Transform.h"
#include <glm/ext/vector_float3.hpp>

namespace dae
{
	class TransformComponent : public Component
	{
	public:
		TransformComponent(dae::GameObject* object, float x = 0, float y = 0);


		const Transform& GetPosition(); //calculates current world pos
		void SetLocalPosition(float x, float y);
		void SetLocalPosition(const glm::vec3& newPos);
		void SetLocalPosition(const Transform& newPos);

	private:
		bool m_ShouldUpdate{ true };
		Transform m_LocalPosition;
		Transform m_WorldPosition;

		void UpdateTransform();
	};
}