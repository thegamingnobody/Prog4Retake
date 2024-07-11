#ifndef CONTROLLER
#define CONTROLLER
#include "InputDevice.h"

namespace dae
{
	class Controller : public InputDevice
	{
	public:

		Controller(int const index);
		~Controller();

		void Update() override;

		bool IsButtonPressed(int const button, const InputType& inputType) const override;

		int GetPlayerNumber() const override;

	private: 
		class ControllerImpl;
		std::unique_ptr<ControllerImpl> m_Impl;
	};

}
#endif 
