#ifndef BUTTONENUMS
#define BUTTONENUMS
#include <SDL_scancode.h>

	namespace dae
	{
		enum class KeyboardKeys
		{
			TR_1 = SDL_SCANCODE_1,
			TR_2 = SDL_SCANCODE_2,
			TR_3 = SDL_SCANCODE_3,
			TR_4 = SDL_SCANCODE_4,
			TR_5 = SDL_SCANCODE_5,
			TR_6 = SDL_SCANCODE_6,
			TR_7 = SDL_SCANCODE_7,
			TR_8 = SDL_SCANCODE_8,
			TR_9 = SDL_SCANCODE_9,
			TR_0 = SDL_SCANCODE_0,

			KP_1 = SDL_SCANCODE_KP_1,
			KP_2 = SDL_SCANCODE_KP_2,
			KP_3 = SDL_SCANCODE_KP_3,
			KP_4 = SDL_SCANCODE_KP_4,
			KP_5 = SDL_SCANCODE_KP_5,
			KP_6 = SDL_SCANCODE_KP_6,
			KP_7 = SDL_SCANCODE_KP_7,
			KP_8 = SDL_SCANCODE_KP_8,
			KP_9 = SDL_SCANCODE_KP_9,
			KP_0 = SDL_SCANCODE_KP_0,

			Q = SDL_SCANCODE_Q,
			W = SDL_SCANCODE_W,
			E = SDL_SCANCODE_E,
			R = SDL_SCANCODE_R,
			T = SDL_SCANCODE_T,
			Y = SDL_SCANCODE_Y,
			U = SDL_SCANCODE_U,
			I = SDL_SCANCODE_I,
			O = SDL_SCANCODE_O,
			P = SDL_SCANCODE_P,
			A = SDL_SCANCODE_A,
			S = SDL_SCANCODE_S,
			D = SDL_SCANCODE_D,
			F = SDL_SCANCODE_F,
			G = SDL_SCANCODE_G,
			H = SDL_SCANCODE_H,
			J = SDL_SCANCODE_J,
			K = SDL_SCANCODE_K,
			L = SDL_SCANCODE_L,
			Z = SDL_SCANCODE_Z,
			X = SDL_SCANCODE_X,
			C = SDL_SCANCODE_C,
			V = SDL_SCANCODE_V,
			B = SDL_SCANCODE_B,
			N = SDL_SCANCODE_N,
			M = SDL_SCANCODE_M,

			LShift = SDL_SCANCODE_LSHIFT,
			RShift = SDL_SCANCODE_RSHIFT,
			LControl = SDL_SCANCODE_LCTRL,
			RControl = SDL_SCANCODE_RCTRL,
			LAlt = SDL_SCANCODE_LALT,
			RAlt = SDL_SCANCODE_RALT,
			CapsLock = SDL_SCANCODE_CAPSLOCK,
			Tab = SDL_SCANCODE_TAB,
			Enter = SDL_SCANCODE_RETURN,
			KP_Enter = SDL_SCANCODE_KP_ENTER,
			Delete = SDL_SCANCODE_DELETE,
			Grave = SDL_SCANCODE_GRAVE,

			F1 = SDL_SCANCODE_F1,
			F2 = SDL_SCANCODE_F2,
			F3 = SDL_SCANCODE_F3,
			F4 = SDL_SCANCODE_F4,
			F5 = SDL_SCANCODE_F5,
			F6 = SDL_SCANCODE_F6,
			F7 = SDL_SCANCODE_F7,
			F8 = SDL_SCANCODE_F8,
			F9 = SDL_SCANCODE_F9,
			F10 = SDL_SCANCODE_F10,
			F11 = SDL_SCANCODE_F11,
			F12 = SDL_SCANCODE_F12,

		};

		enum class ControllerButtons
		{
			DpadUp = 0x0001,
			DpadDown = 0x0002,
			DpadLeft = 0x0004,
			DpadRight = 0x0008,

			Start = 0x0010,
			Select = 0x0020,
			StickLeft = 0x0040,
			StickRight = 0x0080,

			ShoulderLeft = 0x0100,
			ShoulderRight = 0x0200,

			FaceButtonDown	= 0x1000,
			FaceButtonRight	= 0x2000,
			FaceButtonLeft	= 0x4000,
			FaceButtonUp	= 0x8000,
		};

		enum class InputType
		{
			PressedThisFrame,
			Held,
			ReleasedThisFrame
		};
	}

#endif