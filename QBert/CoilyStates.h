#pragma once

#include "State.h"
#include <glm/ext/vector_float3.hpp>
#include <Level.h>


namespace dae
{
	class CoilyJumpingState : public State
	{
	public:
		CoilyJumpingState(GameObject* object, const glm::vec3& direction, float jumpTime, bool isTileValid);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

	private:
		float const m_MaxJumpTime;
		float m_AccumulatedTime;
		glm::vec3 m_Direction;
		bool m_IsTileValid;
	};

	class CoilyRequestingMovementState : public State, public Observer
	{
	public:
		CoilyRequestingMovementState(GameObject* object);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

		void Notify(const Event& event) override;
	};

	class CoilyFinishMovementState : public State, public Observer
	{
	public:
		CoilyFinishMovementState(GameObject* object);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

		void Notify(const Event& event) override;
	};

	class CoilyIdleState : public State, public Observer
	{
	public:
		CoilyIdleState(GameObject* object);

		void OnEnter() override;
		void Update(float const deltaTime) override;
		void OnExit() override;

		void Notify(const Event& event) override;
	private:
		int CalculateNextDirectionID();

		float m_AccumulatedTime;

		std::vector<glm::vec3> m_Directions;
	};

}


