#ifndef COUNTERCOMPONENT
#define COUNTERCOMPONENT

#include "Component.h"
#include <string>
#include <map>
#include "Observer.h"

namespace dae 
{
	enum class CounterTypes
	{
		Lives,
		Score,
		Custom
	};

	class CounterComponent : public Component, public Observer
	{
	public:
		void Update(float const elapsedTime) override;

		void AddCounter(const std::string& counterName, const CounterTypes& counterType, int const maxValue = 0, bool const capAtMaxValue = false);

		void AddValue(const std::string& counterName, int const valueToAdd);
		void DecreaseValue(const std::string& counterName, int const valueToDecrease);

		int GetCurrentValue(const std::string& counterName);
		int GetMaxValue(const std::string& counterName);

		CounterComponent(dae::GameObject* object);

		void Notify(const Event& event) override;

	private:
		struct Counter
		{
			int m_CurrentValue;
			int m_MaxValue;
			bool m_CapAtMax;
			CounterTypes m_Type;
		};
		std::map<std::string, Counter> m_Counters;
	};
}


#endif