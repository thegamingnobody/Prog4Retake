#include "QBertStates.h"
#include "GameObject.h"
#include <TransformComponent.h>

dae::JumpingState::JumpingState(GameObject* object, const glm::vec3& endPos)
	: State(object)
	, m_MaxJumpTime()
	, m_AccumulatedTime()
	, m_StartPos()
	, m_EndPos(endPos)
{
}

void dae::JumpingState::OnEnter()
{
	m_AccumulatedTime = 0.0f;
	auto object = GetObject();

	if (object)
	{
		auto transformCpnt = object->GetComponent<dae::TransformComponent>();
		if (transformCpnt)
		{
			m_StartPos = transformCpnt->GetPosition().GetPosition();
		}
	}
}

void dae::JumpingState::Update(float const deltaTime)
{
	m_AccumulatedTime += deltaTime;
}

void dae::JumpingState::OnExit()
{
}
