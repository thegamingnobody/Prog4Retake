#include "Transform.h"

dae::Transform::Transform(float x, float y, float z)
	: m_position(glm::vec3(x, y, z))
{
}

dae::Transform::Transform(const glm::vec3& pos)
	: m_position(pos)
{
}

void dae::Transform::SetPosition(const float x, const float y, const float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void dae::Transform::SetPosition(const glm::vec3& pos)
{
	SetPosition(pos.x, pos.y, pos.z);
}

void dae::Transform::SetPosition(const Transform& other)
{
	SetPosition(other.GetPosition().x, other.GetPosition().y, other.GetPosition().z);
}

dae::Transform dae::Transform::operator-(const Transform& other) const
{
	return Transform(m_position.x - other.m_position.x, m_position.y - other.m_position.y, m_position.z - other.m_position.z);
}

dae::Transform dae::Transform::operator+(const Transform& other) const
{
	return Transform(m_position - (-other.m_position));
}
