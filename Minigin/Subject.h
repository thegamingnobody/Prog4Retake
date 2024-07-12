#ifndef DAE_SUBJECT
#define DAE_SUBJECT

#include "Observer.h"
#include <iostream>
#include <vector>
#include "Event.h"

namespace dae
{
	class Subject final
	{
	public:
		Subject() = default;

		void AddObserver(Observer* observer, EventType& eventType)
		{
			if (observer)
			{
				m_Observers.emplace_back(std::make_pair(eventType, observer));
			}
		}

		void RemoveObserver(Observer* observer)
		{
			auto removedObserverIt{ std::remove_if(m_Observers.begin(), m_Observers.end(), [observer](const std::pair<const EventType, Observer*>& pair)
				{
					return pair.second == observer;
				})};

			if (removedObserverIt != m_Observers.end())
			{
				m_Observers.erase(removedObserverIt);
			}
		}

		void NotifyObservers(const Event& event)
		{
			for (auto& observer : m_Observers)
			{
				if (event.m_type == observer.first)
				{
					int const targetNumber{ observer.second->GetTargetNumber() };
					if (event.m_playerNumber == targetNumber or event.m_playerNumber == -1 or targetNumber == -1)
					{
						observer.second->Notify(event);
					}
				}
			}
		}

	private:
		std::vector< std::pair<EventType, Observer*> > m_Observers{};
	};
}

#endif