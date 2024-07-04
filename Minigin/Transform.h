#pragma once
#include <glm/glm.hpp>

namespace dae
{
	class Transform final
	{
	public:
		Transform(float x, float y, float z);
		Transform(const glm::vec3& pos);

		const glm::vec3& GetPosition() const { return m_position; }
		void SetPosition(float x, float y, float z);
		void SetPosition(const glm::vec3& pos);
		void SetPosition(const Transform& other);

		Transform operator-(const Transform& other) const;
		Transform operator+(const Transform& other) const;
	private:
		glm::vec3 m_position;
	};
}
