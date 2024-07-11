#ifndef DAE_INPUTDEVICE
#define DAE_INPUTDEVICE
#include <memory>
#include "ButtonEnums.h"

namespace dae
{
	class InputDevice
	{
	public:
		virtual ~InputDevice() = default;
		virtual void Update() = 0;
		virtual int GetPlayerNumber() const = 0;
		virtual bool IsButtonPressed(int const button, const InputType& inputType) const = 0;
	};	
}

#endif