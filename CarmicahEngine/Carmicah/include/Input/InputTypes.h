#ifndef INPUT_TYPES_H
#define INPUT_TYPES_H

#include <Windows.h>
#include <GLFW/glfw3.h>

namespace Carmicah
{
	enum Keys
	{
		// KEY_ = 0x,

		// Misc Keys
		KEY_BACKSPACE = 0x08,
		KEY_TAB = 0x09,
		KEY_ENTER = 0x0D,
		KEY_CTRL = 0x11,
		KEY_CAPSLOCK = 0x14,
		KEY_ESC = 0x1B,
		KEY_SPACEBAR = 0x20,

		// Arrow Keys
		KEY_ARROW_LEFT = 0x21, // PAGE UP key
		KEY_ARROW_UP = 0x22,	// PAGE DOWN key
		KEY_ARROW_RIGHT = 0x23,	// END key
		KEY_ARROW_DOWN = 0x24,	// HOME key

		// Misc Keys
		KEY_PRINT_SCREEN = 0x2C,
		KEY_DELETE = 0x2E,
		KEY_SHIFT_LEFT = 0xA2,
		KEY_SHIFT_RIGHT = 0xA3,
		KEY_ALT_LEFT = 0xA4,
		KEY_ALT_RIGHT = 0xA5,

		// Volume Keys
		KEY_VOLUME_MUTE = 0xAD,
		KEY_VOLUME_DOWN = 0xAE,
		KEY_VOLUME_UP = 0xAF,

		// Misc Keys
		KEY_PLUS = 0xBB,
		KEY_COMMA = 0xBC,
		KEY_DASH = 0xBD,
		KEY_FULLSTOP = 0xBE,

		// Number Keys
		KEY_0 = 0x30,
		KEY_1 = 0x31,
		KEY_2 = 0x32,
		KEY_3 = 0x33,
		KEY_4 = 0x34,
		KEY_5 = 0x35,
		KEY_6 = 0x36,
		KEY_7 = 0x37,
		KEY_8 = 0x38,
		KEY_9 = 0x39,

		// Alphabet Keys
		KEY_A = 0x41,
		KEY_B = 0x42,
		KEY_C = 0x43,
		KEY_D = 0x44,
		KEY_E = 0x45,
		KEY_F = 0x46,
		KEY_G = 0x47,
		KEY_H = 0x48,
		KEY_I = 0x49,
		KEY_J = 0x4A,
		KEY_K = 0x4B,
		KEY_L = 0x4C,
		KEY_M = 0x4D,
		KEY_N = 0x4E,
		KEY_O = 0x4F,
		KEY_P = 0x50,
		KEY_Q = 0x51,
		KEY_R = 0x52,
		KEY_S = 0x53,
		KEY_T = 0x54,
		KEY_U = 0x55,
		KEY_V = 0x56,
		KEY_W = 0x57,
		KEY_X = 0x58,
		KEY_Y = 0x59,
		KEY_Z = 0x5A,

		// F Keys
		KEY_F01 = 0x70,
		KEY_F02 = 0x71,
		KEY_F03 = 0x72,
		KEY_F04 = 0x73,
		KEY_F05 = 0x74,
		KEY_F06 = 0x75,
		KEY_F07 = 0x76,
		KEY_F08 = 0x77,
		KEY_F09 = 0x78,
		KEY_F10 = 0x79,
		KEY_F11 = 0x7A,
		KEY_F12 = 0x7B,

		// MAX_KEYBOARD_KEYS,
	};

	enum MouseButtons
	{
		MOUSE_BUTTON_LEFT = 0,
		MOUSE_BUTTON_RIGHT,
		MOUSE_BUTTON_WHEEL,
		MAX_MOUSE_BUTONS,
	};

	enum KeyStates
	{
		NONE = -1,
		RELEASE,
		PRESSED,
		HOLD
	};
}

#endif
