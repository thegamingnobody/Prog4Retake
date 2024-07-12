#ifndef DAE_OBSERVER
#define DAE_OBSERVER

#include <tuple>
#include <any>
#include "Event.h"

namespace dae
{
	class Observer
	{
	public:
		virtual ~Observer() {};

		virtual void Notify(const Event& event) = 0;
		int GetTargetNumber() { return m_TargetNumber; }
	protected:
		int m_TargetNumber{-1};
 	};
}

#endif