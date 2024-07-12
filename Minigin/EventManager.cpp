#include "EventManager.h"

void dae::EventManager::Init()
{
	m_Subject = std::make_unique<Subject>();
}

void dae::EventManager::ProcessQueue()
{
	while (not m_EventQueue.empty())
	{
		ProcessEvent(m_EventQueue.front());
		m_EventQueue.pop();
	}
	
}

void dae::EventManager::PushEvent(const Event& event)
{
	m_EventQueue.push(event);
}

void dae::EventManager::AddObserver(Observer* observer, EventType eventType)
{
	m_Subject->AddObserver(observer, eventType);
}

void dae::EventManager::RemoveObserver(Observer* observer)
{
	m_Subject->RemoveObserver(observer);
}
