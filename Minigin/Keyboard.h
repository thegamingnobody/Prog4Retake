#ifndef KEYBOARD
#define KEYBOARD

#include "InputDevice.h"

namespace dae
{
	class Keyboard : public InputDevice
	{
	public:

		Keyboard(int const index);
		~Keyboard();

		void Update() override;

		bool IsButtonPressed(const int button, const InputType& inputType) const override;

		int GetPlayerNumber() const override;

	private:
		class KeyboardImpl;
		std::unique_ptr<KeyboardImpl> m_Impl;
	};
}

#endif