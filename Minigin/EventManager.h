#ifndef DAE_EVENTMANAGER
#define DAE_EVENTMANAGER

#include "Singleton.h"
#include "Event.h"
#include <memory>
#include "Subject.h"
#include <queue>
#include <any>


namespace dae
{
	class EventManager final : public Singleton<EventManager>
	{
	public:
		void Init();

		void ProcessQueue();

		void PushEvent(const Event& event);

		void AddObserver(Observer* observer, EventType eventType);
		void RemoveObserver(Observer* observer);


	private:
		friend class Singleton<EventManager>;
		EventManager() = default;
		std::unique_ptr<Subject> m_Subject;

		std::queue<Event> m_EventQueue;

		void ProcessEvent(const Event& event)
		{
			m_Subject->NotifyObservers(event);
		}
	};
}

#endif