#ifndef QBERT_STATES
#define QBERT_STATES

#include "State.h"
#include <glm/ext/vector_float3.hpp>
#include <Level.h>

namespace dae
{
	class JumpingState : public State
	{
	public:
		JumpingState(GameObject* object, const glm::vec3& direction, float jumpTime, bool isTileValid);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

	private:
		float const m_MaxJumpTime;
		float m_AccumulatedTime;
		glm::vec3 m_Direction;
		bool m_IsTileValid;
	};

	class RequestingMovementState : public State, public Observer
	{
	public:
		RequestingMovementState(GameObject* object);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

		void Notify(const Event& event) override;
	};

	class FinishMovementState : public State, public Observer
	{
	public:
		FinishMovementState(GameObject* object);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

		void Notify(const Event& event) override;
	};

	class IdleState : public State, public Observer
	{
	public:
		IdleState(GameObject* object);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

		void Notify(const Event& event) override;
	};

	class DeathState : public State
	{
	public:
		DeathState(GameObject* object, float deathTimer);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

	private:
		float m_DeathTimer;
		float m_AccumulatedTime;
	};


	 
}

#endif
