#ifndef QBERT_STATES
#define QBERT_STATES

#include "State.h"
#include <glm/ext/vector_float3.hpp>

namespace dae
{
	class JumpingState : public State
	{
	public:
		JumpingState(GameObject* object, const glm::vec3& endPos);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

	private:
		float const m_MaxJumpTime;
		float m_AccumulatedTime;
		glm::vec3 m_StartPos;
		glm::vec3 m_EndPos;

	};
}

#endif
