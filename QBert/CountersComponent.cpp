#include "CountersComponent.h"
#include "GameObject.h"
#include "EventManager.h"
#include <ServiceLocator.h>

void dae::CounterComponent::Update(float const)
{
}

void dae::CounterComponent::AddCounter(const std::string& counterName, const CounterTypes& counterType, int const maxValue, bool const capAtMaxValue)
{
	Counter temp{};
	temp.m_CurrentValue = maxValue;
	temp.m_MaxValue = maxValue;
	temp.m_CapAtMax = capAtMaxValue;
	temp.m_Type = counterType;

	m_Counters.insert({ counterName, temp });
	
}

void dae::CounterComponent::AddValue(const std::string& counterName, int const valueToAdd)
{
	auto counter{ m_Counters.find(counterName) };

	if (counter != m_Counters.end())
	{
		counter->second.m_CurrentValue += valueToAdd;

		if (counter->second.m_CapAtMax and counter->second.m_CurrentValue > counter->second.m_MaxValue)
		{
			counter->second.m_CurrentValue = counter->second.m_MaxValue;
		}

		std::cout << counterName << ": " << counter->second.m_CurrentValue << "\n";
	}
	else
	{
		throw std::exception("Counter not found");
	}
}

void dae::CounterComponent::DecreaseValue(const std::string& counterName, int const valueToDecrease)
{
	AddValue(counterName, -valueToDecrease);
}

int dae::CounterComponent::GetCurrentValue(const std::string& counterName)
{
	auto counter{ m_Counters.find(counterName) };

	if (counter != m_Counters.end())
	{
		return counter->second.m_CurrentValue;
	}

	throw std::exception("Counter not found");
}

int dae::CounterComponent::GetMaxValue(const std::string& counterName)
{
	auto counter{ m_Counters.find(counterName) };

	if (counter != m_Counters.end())
	{
		return counter->second.m_MaxValue;
	}

	throw std::exception("Counter not found");

}

dae::CounterComponent::CounterComponent(dae::GameObject* object)
	: Component(object)
	, m_Counters()
{
	m_TargetNumber = object->GetObjectID();
	//dae::EventManager::GetInstance().AddObserver(this, dae::EventType::PlayerDied);
}

void dae::CounterComponent::Notify(const Event& /*event*/)
{
	//auto castedArguments{ event.GetArgumentsAsTuple<const std::string&, int const>()};
	//AddValue(std::get<0>(castedArguments), std::get<1>(castedArguments));
	//std::cout << std::get<0>(castedArguments) << ": " << GetCurrentValue(std::get<0>(castedArguments)) << "\n";
}
