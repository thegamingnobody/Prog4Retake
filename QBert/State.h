#ifndef DAE_STATE
#define DAE_STATE

#include <Component.h>

namespace dae
{
	class State
	{
	public:
		
		virtual ~State() = default;
		State(const State& other) = delete;
		State(State&& other) = delete;
		State& operator=(const State& other) = delete;
		State& operator=(State&& other) = delete;


		virtual void OnEnter() {}
		virtual void Update(float const /*deltaTime*/) {}
		virtual void OnExit() {}

	protected:
		State(GameObject* object) : m_pObjectOfState(object) {}
		GameObject* GetObject() const { return m_pObjectOfState; }

	private:
		GameObject* m_pObjectOfState{};

	};
}

#endif