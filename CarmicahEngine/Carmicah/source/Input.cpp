#pragma region C Rendition of Input.cpp
/*


#include "Systems//Input.h"
#include "Systems/Events.h"
#include <iostream>


//-------------------------------------------------------------------------------------------------
#pragma region Global Structs
//-------------------------------------------------------------------------------------------------
// this struct will keep track of what key is currently being pressed
typedef struct Keyboard_State
{
	bool keys[256];
}Keyboard_State;

//-------------------------------------------------------------------------------------------------
// Keeps track of position of mouse and the buttons
typedef struct Mouse_State
{
	i16 x; // mouse positions
	i16 y; // mouse positions
	bool buttons[MAX_MOUSE_BUTONS];
	bool dragging[MAX_MOUSE_BUTONS]; // want to keep track if mouse button is being held down and mouse is moving
}Mouse_State;


//-------------------------------------------------------------------------------------------------
// this will keep track of what the keyboard state and mouse state is
typedef struct InputSystem_State
{
	Keyboard_State keyboard_current;	// this keeps track of the current buttons being pressed
	Keyboard_State keyboard_previous;	// this keeps track of the previous buttons that were pressed
	Mouse_State mouse_current;			// keeps track of current mouse pos and buttons
	Mouse_State mouse_previous;			// keeps track of previous mouse pos and buttons pressed

	bool keys_repeats_enabled;			// i want functionality that allows repeated keys
}InputSystem_State;


// static global state for the (internal to this file) input system
static InputSystem_State* iState;


#pragma endregion


//-------------------------------------------------------------------------------------------------
#pragma region System Init & Shutdown

//-------------------------------------------------------------------------------------------------
bool input_system_initialise()
{
	// initialise the system

	// check if iState has been init'd, if it has been, do nothing, if it hasn't, allocate memory
	if (!iState)
	{
		// assign mem using new, c++ style
		iState = new InputSystem_State();
		std::cout << "Input System has been initialised!" << std::endl;
	}

	// set this to false by default, so it can be enabled in other parts of the system
	iState->keys_repeats_enabled = false;

	return true;
}


//-------------------------------------------------------------------------------------------------
void input_system_shutdown()
{
	// free memory
	free(iState);
	iState = NULL;
}

#pragma endregion


#pragma region Input Key Checks & Processing

//-------------------------------------------------------------------------------------------------
bool input_is_key_down(Keys key)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	// check if this key is currently down, if yes, return true, if not return false
	return iState->keyboard_current.keys[key] == true;
}



//-------------------------------------------------------------------------------------------------
bool input_is_key_up(Keys key)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	// check if this key is currently up, if yes, return false, if not return true
	return iState->keyboard_current.keys[key] == false;
}


//-------------------------------------------------------------------------------------------------
bool input_was_key_down(Keys key)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	// check if this key is currently down, if yes, return true, if not return false
	return iState->keyboard_previous.keys[key] == true;
}



//-------------------------------------------------------------------------------------------------
bool input_was_key_up(Keys key)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	// check if this key is currently up, if yes, return false, if not return true
	return iState->keyboard_previous.keys[key] == false;
}



//-------------------------------------------------------------------------------------------------
void input_process_keys(Keys key, bool pressed)
{
	if (!iState)
	{
		return;
	}

	// check if key is repeating and being pressed
	bool isRepeating = pressed && iState->keyboard_current.keys[key];
	// check if it is NOT pressed
	bool changed = (iState->keyboard_current.keys[key] != pressed);
	
	// verytime a key is pressed or released, fire an event
	//	in this loop we are checking if a key has been pressed/released
	//	everytime we check for that, we fire an event
	//	any subscriber listening to that event can than handle that event 
	if (iState->keys_repeats_enabled || changed) // check if iState repeat state is enabled
	{
		// if the key we're currently pressing is not the same as the key previously pressed, update the keyboard state
		// Update state
		iState->keyboard_current.keys[key] = pressed;

		// send this data to the subscriber listening to the event fired by this
		EventData event_data;
		// storing the key data in this first register of this i16 array
		event_data.data.u16[0] = key; 
		// check if the key is repeating, if it is, store a 1, if not, store a 0
		event_data.data.u16[1] = isRepeating ? 1 : 0; 
		// if it is pressed, send pressed code key, if not send key released. send event_data which contains the specific key and whether its repeating or not
		event_publish(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, 0, event_data);
		// event is now ready to be published
	}
}

#pragma endregion


#pragma region Input Mouse Checks & Processing

//-------------------------------------------------------------------------------------------------
bool input_is_mouse_down(Mouse_Buttons button)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	return iState->mouse_current.buttons[button] == true;
}


//-------------------------------------------------------------------------------------------------
bool input_is_mouse_up(Mouse_Buttons button)
{	
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	return iState->mouse_current.buttons[button] == false;
}


//-------------------------------------------------------------------------------------------------
bool input_was_mouse_down(Mouse_Buttons button)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	return iState->mouse_previous.buttons[button] == true;
}


//-------------------------------------------------------------------------------------------------
bool input_was_mouse_up(Mouse_Buttons button)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	return iState->mouse_previous.buttons[button] == false;
}


//-------------------------------------------------------------------------------------------------
bool input_is_mouse_button_dragging(Mouse_Buttons button)
{
	// sanity check that there is a valid input system set up
	if (!iState)
	{
		return false;
	}

	// check if this key is currently up, if yes, return false, if not return true
	return iState->mouse_current.dragging[button] == true;
}


//-------------------------------------------------------------------------------------------------
void input_get_mouse_position(i16* x, i16* y)
{
	// return the mouse position
	// if state not init'd, set x and y to 0
	if (!iState)
	{
		*x = 0;
		*y = 0;
		return;
	}

	// if it IS init'd, set coordinate to whatever is passed in
	*x = iState->mouse_current.x;
	*y = iState->mouse_current.y;
}


//-------------------------------------------------------------------------------------------------
void input_get_previous_mouse_position(i16* x, i16* y)
{
	// if state not init'd, set x and y to 0
	if (!iState)
	{
		*x = 0;
		*y = 0;
		return;
	}

	// if it IS init'd, set coordinate to whatever is passed in
	*x = iState->mouse_previous.x;
	*y = iState->mouse_previous.y;
}


//-------------------------------------------------------------------------------------------------
void input_process_mouse_button(Mouse_Buttons button, bool pressed)
{
	if (!iState)
	{
		return;
	}

	// if the state changed, publish an event
	if (iState->mouse_current.buttons[button] != pressed)
	{
		// if it is not pressed, set the button to pressed
		iState->mouse_current.buttons[button] = pressed;

		
		EventData event_data; // send this data to the subscriber listening to the event fired by this
		event_data.data.u16[0] = button; // storing the button data in this first register of this i16 array
		event_data.data.u16[1] = iState->mouse_current.x; // store the x and y positions in the array
		event_data.data.u16[2] = iState->mouse_current.y;
		// if it is pressed, send pressed code button, if not send button released. send event_data which contains the specific button
		event_publish(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_MOUSE_BUTTON_RELEASED, 0, event_data); // event is now ready to be published/fired
	}

	// Handle drag release if any
	// draggin logic is handled in input_process_mouse_move
	// this checks if mouse is being released, and if it is being released, it stops any drag operation that is
	// currently active
	if (!pressed) // -> is this button is NOT pressed and IS currently dragging, stop dragging
	{
		if (iState->mouse_current.dragging[button])
		{
			iState->mouse_current.dragging[button] = false;

			// publish a mouse drag-end event
			EventData event_data;
			event_data.data.u16[0] = button;
			event_data.data.u16[1] = iState->mouse_current.x;
			event_data.data.u16[2] = iState->mouse_current.y;
			event_publish(EVENT_CODE_MOUSE_DRAG_END, 0, event_data);
		}
		else
		{
			// if its not a drag release, it is a click event
			// publish a mouse click event
			EventData event_data;
			event_data.data.u16[0] = button;
			event_data.data.u16[1] = iState->mouse_current.x;
			event_data.data.u16[2] = iState->mouse_current.y;
			event_publish(EVENT_CODE_MOUSE_BUTTON_CLICKED, 0, event_data);
		}
	}
}


//-------------------------------------------------------------------------------------------------
void input_process_mouse_move(i16 x, i16 y)
{
	if (!iState)
	{
		return;
	}

	// check if the state changed
	if (iState->mouse_current.x != x || iState->mouse_current.y != y)
	{
		// update the x and y position
		iState->mouse_current.x = x;
		iState->mouse_current.y = y;

		EventData event_data;
		event_data.data.u16[0] = x;
		event_data.data.u16[1] = y;
		event_publish(EVENT_CODE_MOUSE_MOVED, 0, event_data);


		// handle draggin operations 
		for (int i{}; i < MAX_MOUSE_BUTONS; ++i)
		{
			// check if the current mouse button is pressed/down
			if (iState->mouse_current.buttons[i])
			{
				// then check if the mouse button is not currently dragging and previously not dragging,
				// then i want to start dragging
				if (!iState->mouse_previous.dragging[i] && !iState->mouse_current.dragging[i])
				{
					// start dragging the button
					iState->mouse_current.dragging[i] = true;

					// publish a mouse drag event
					EventData event_data;
					event_data.data.u16[0] = i;
					event_data.data.u16[1] = iState->mouse_current.x;
					event_data.data.u16[2] = iState->mouse_current.y;
					event_publish(EVENT_CODE_MOUSE_DRAG_BEGIN, 0, event_data);
				}
				else if(iState->mouse_current.dragging[i]) // check if we currently ARE dragging, if we are, just continue dragging
				{
					// publish a continuance of the drag operation event
					EventData event_data;
					event_data.data.u16[0] = i;
					event_data.data.u16[1] = iState->mouse_current.x;
					event_data.data.u16[2] = iState->mouse_current.y;
					event_publish(EVENT_CODE_MOUSE_DRAGGED, 0, event_data);
				}
			}
		}
	}
}


//-------------------------------------------------------------------------------------------------

void input_process_mouse_wheel(i8 delta_z)
{
	// PUBLISH a mouse wheel 
	EventData event_data;
	event_data.data.i8[0] = delta_z;
	event_publish(EVENT_CODE_MOUSE_WHEEL, 0, event_data);
}


#pragma endregion


//-------------------------------------------------------------------------------------------------
#pragma region input_keycode_to_string
// this parses and returns what our keys map to, it returns the name of the specific key
const char* input_keycode_to_string(Keys key)
//std::string input_keycode_to_string(Keys key) -> can this work?
{
	switch (key)
	{
		// Misc Keys
	case KEY_BACKSPACE:
		return "backspace";
	case KEY_TAB:
		return "tab";
	case KEY_ENTER:
		return "enter";
	case KEY_CTRL:
		return "ctrl";
	case KEY_CAPSLOCK:
		return "capslock";
	case KEY_ESC:
		return "esc";
	case KEY_SPACEBAR:
		return "spacebar";
	case KEY_PLUS:
		return "plus";
	case KEY_COMMA:
		return "comma";
	case KEY_DASH:
		return "dash";
	case KEY_FULLSTOP:
		return "fullstop";
	case KEY_PRINT_SCREEN:
		return "print_screen";
	case KEY_DELETE:
		return "delete";
	case KEY_SHIFT_LEFT:
		return "shift_left";
	case KEY_SHIFT_RIGHT:
		return "shift_right";
	case KEY_ALT_LEFT:
		return "alt_left";
	case KEY_ALT_RIGHT:
		return "alt_right";

		// Arrow Keys
	case KEY_ARROW_LEFT:
		return "arrow_left";
	case KEY_ARROW_UP:
		return "arrow_up";
	case KEY_ARROW_RIGHT:
		return "arrow_right";
	case KEY_ARROW_DOWN:
		return "arrow_down";

		// Volume Keys
	case KEY_VOLUME_MUTE:
		return "volume_mute";
	case KEY_VOLUME_DOWN:
		return "volume_down";
	case KEY_VOLUME_UP:
		return "volume_up";

		// Number Keys
	case KEY_0:
		return "0";
	case KEY_1:
		return "1";
	case KEY_2:
		return "2";
	case KEY_3:
		return "3";
	case KEY_4:
		return "4";
	case KEY_5:
		return "5";
	case KEY_6:
		return "6";
	case KEY_7:
		return "7";
	case KEY_8:
		return "8";
	case KEY_9:
		return "9";

		// Alphabet Keys
	case KEY_A:
		return "A";
	case KEY_B:
		return "B";
	case KEY_C:
		return "C";
	case KEY_D:
		return "D";
	case KEY_E:
		return "E";
	case KEY_F:
		return "F";
	case KEY_G:
		return "G";
	case KEY_H:
		return "H";
	case KEY_I:
		return "I";
	case KEY_J:
		return "J";
	case KEY_K:
		return "K";
	case KEY_L:
		return "L";
	case KEY_M:
		return "M";
	case KEY_N:
		return "N";
	case KEY_O:
		return "O";
	case KEY_P:
		return "P";
	case KEY_Q:
		return "Q";
	case KEY_R:
		return "R";
	case KEY_S:
		return "S";
	case KEY_T:
		return "T";
	case KEY_U:
		return "U";
	case KEY_V:
		return "V";
	case KEY_W:
		return "W";
	case KEY_X:
		return "X";
	case KEY_Y:
		return "Y";
	case KEY_Z:
		return "Z";

		// Function Keys
	case KEY_F01:
		return "F1";
	case KEY_F02:
		return "F2";
	case KEY_F03:
		return "F3";
	case KEY_F04:
		return "F4";
	case KEY_F05:
		return "F5";
	case KEY_F06:
		return "F6";
	case KEY_F07:
		return "F7";
	case KEY_F08:
		return "F8";
	case KEY_F09:
		return "F9";
	case KEY_F10:
		return "F10";
	case KEY_F11:
		return "F11";
	case KEY_F12:
		return "F12";

	default:
		return "undefined";
	}
}


*/
#pragma endregion



#pragma region C++ Rendition of Input.cpp
///*


#include "Systems//Input.h"
#include "Systems/Events.h"

// Constructor that takes in a reference to EventSystem
InputSystem::InputSystem(EventSystem& eventSystem) : event_system(eventSystem) {}

// Destructor
InputSystem::~InputSystem() {
	shutdown();
}

// Initialize the input system
bool InputSystem::initialise() {
	state = std::make_unique<InputState>();
	std::cout << "Input System has been initialized!" << std::endl;
	return true;
}

// Shutdown and clean up
void InputSystem::shutdown() {
	state.reset();  // Smart pointer takes care of memory deallocation
}

// Key state checks
bool InputSystem::is_key_down(Keys key) {
	return state ? state->current_keyboard.keys[key] : false;
}

bool InputSystem::is_key_up(Keys key) {
	return state ? !state->current_keyboard.keys[key] : false;
}

bool InputSystem::was_key_down(Keys key) {
	return state ? state->previous_keyboard.keys[key] : false;
}

bool InputSystem::was_key_up(Keys key) {
	return state ? !state->previous_keyboard.keys[key] : false;
}

// Process key presses and releases
void InputSystem::process_keys(Keys key, bool pressed) {
	if (!state) return;

	bool changed = (state->current_keyboard.keys[key] != pressed);
	bool isRepeating = pressed && state->current_keyboard.keys[key];

	if (state->keys_repeats_enabled || changed) {
		state->current_keyboard.keys[key] = pressed;

		EventData event_data;
		event_data.data.u16[0] = key;
		event_data.data.u16[1] = isRepeating ? 1 : 0;

		// Publish key pressed or released event
		event_system.publish(pressed ? EVENT_CODE_KEY_PRESSED : EVENT_CODE_KEY_RELEASED, nullptr, event_data);
	}
}

// Mouse state checks
bool InputSystem::is_mouse_down(Mouse_Buttons button) {
	return state ? state->current_mouse.buttons[button] : false;
}

bool InputSystem::is_mouse_up(Mouse_Buttons button) {
	return state ? !state->current_mouse.buttons[button] : false;
}

bool InputSystem::was_mouse_down(Mouse_Buttons button) {
	return state ? state->previous_mouse.buttons[button] : false;
}

bool InputSystem::was_mouse_up(Mouse_Buttons button) {
	return state ? !state->previous_mouse.buttons[button] : false;
}

bool InputSystem::is_mouse_button_dragging(Mouse_Buttons button) {
	return state ? state->current_mouse.dragging[button] : false;
}

// Get mouse position
void InputSystem::get_mouse_position(i16& x, i16& y) {
	if (state) {
		x = state->current_mouse.x;
		y = state->current_mouse.y;
	}
	else {
		x = y = 0;
	}
}

// Get previous mouse position
void InputSystem::get_previous_mouse_position(i16& x, i16& y) {
	if (state) {
		x = state->previous_mouse.x;
		y = state->previous_mouse.y;
	}
	else {
		x = y = 0;
	}
}

// Process mouse button actions
void InputSystem::process_mouse_button(Mouse_Buttons button, bool pressed) {
	if (!state) return;

	if (state->current_mouse.buttons[button] != pressed) {
		state->current_mouse.buttons[button] = pressed;

		EventData event_data;
		event_data.data.u16[0] = button;
		event_data.data.u16[1] = state->current_mouse.x;
		event_data.data.u16[2] = state->current_mouse.y;

		event_system.publish(pressed ? EVENT_CODE_MOUSE_BUTTON_PRESSED : EVENT_CODE_MOUSE_BUTTON_RELEASED, nullptr, event_data);
	}
}

// Process mouse movement
void InputSystem::process_mouse_move(i16 x, i16 y) {
	if (!state) return;

	if (state->current_mouse.x != x || state->current_mouse.y != y) {
		state->current_mouse.x = x;
		state->current_mouse.y = y;

		EventData event_data;
		event_data.data.u16[0] = x;
		event_data.data.u16[1] = y;
		event_system.publish(EVENT_CODE_MOUSE_MOVED, nullptr, event_data);

		// Handle dragging
		for (int i = 0; i < MAX_MOUSE_BUTONS; ++i) {
			if (state->current_mouse.buttons[i]) {
				if (!state->current_mouse.dragging[i]) {
					state->current_mouse.dragging[i] = true;
					event_system.publish(EVENT_CODE_MOUSE_DRAG_BEGIN, nullptr, event_data);
				}
				else {
					event_system.publish(EVENT_CODE_MOUSE_DRAGGED, nullptr, event_data);
				}
			}
		}
	}
}

// Process mouse wheel actions
void InputSystem::process_mouse_wheel(i8 delta_z) {
	EventData event_data;
	event_data.data.i8[0] = delta_z;
	event_system.publish(EVENT_CODE_MOUSE_WHEEL, nullptr, event_data);
}

// Convert keycode to string
const char* InputSystem::keycode_to_string(Keys key) {
	switch (key)
	{
		// Misc Keys
	case KEY_BACKSPACE:
		return "backspace";
	case KEY_TAB:
		return "tab";
	case KEY_ENTER:
		return "enter";
	case KEY_CTRL:
		return "ctrl";
	case KEY_CAPSLOCK:
		return "capslock";
	case KEY_ESC:
		return "esc";
	case KEY_SPACEBAR:
		return "spacebar";
	case KEY_PLUS:
		return "plus";
	case KEY_COMMA:
		return "comma";
	case KEY_DASH:
		return "dash";
	case KEY_FULLSTOP:
		return "fullstop";
	case KEY_PRINT_SCREEN:
		return "print_screen";
	case KEY_DELETE:
		return "delete";
	case KEY_SHIFT_LEFT:
		return "shift_left";
	case KEY_SHIFT_RIGHT:
		return "shift_right";
	case KEY_ALT_LEFT:
		return "alt_left";
	case KEY_ALT_RIGHT:
		return "alt_right";

		// Arrow Keys
	case KEY_ARROW_LEFT:
		return "arrow_left";
	case KEY_ARROW_UP:
		return "arrow_up";
	case KEY_ARROW_RIGHT:
		return "arrow_right";
	case KEY_ARROW_DOWN:
		return "arrow_down";

		// Volume Keys
	case KEY_VOLUME_MUTE:
		return "volume_mute";
	case KEY_VOLUME_DOWN:
		return "volume_down";
	case KEY_VOLUME_UP:
		return "volume_up";

		// Number Keys
	case KEY_0:
		return "0";
	case KEY_1:
		return "1";
	case KEY_2:
		return "2";
	case KEY_3:
		return "3";
	case KEY_4:
		return "4";
	case KEY_5:
		return "5";
	case KEY_6:
		return "6";
	case KEY_7:
		return "7";
	case KEY_8:
		return "8";
	case KEY_9:
		return "9";

		// Alphabet Keys
	case KEY_A:
		return "A";
	case KEY_B:
		return "B";
	case KEY_C:
		return "C";
	case KEY_D:
		return "D";
	case KEY_E:
		return "E";
	case KEY_F:
		return "F";
	case KEY_G:
		return "G";
	case KEY_H:
		return "H";
	case KEY_I:
		return "I";
	case KEY_J:
		return "J";
	case KEY_K:
		return "K";
	case KEY_L:
		return "L";
	case KEY_M:
		return "M";
	case KEY_N:
		return "N";
	case KEY_O:
		return "O";
	case KEY_P:
		return "P";
	case KEY_Q:
		return "Q";
	case KEY_R:
		return "R";
	case KEY_S:
		return "S";
	case KEY_T:
		return "T";
	case KEY_U:
		return "U";
	case KEY_V:
		return "V";
	case KEY_W:
		return "W";
	case KEY_X:
		return "X";
	case KEY_Y:
		return "Y";
	case KEY_Z:
		return "Z";

		// Function Keys
	case KEY_F01:
		return "F1";
	case KEY_F02:
		return "F2";
	case KEY_F03:
		return "F3";
	case KEY_F04:
		return "F4";
	case KEY_F05:
		return "F5";
	case KEY_F06:
		return "F6";
	case KEY_F07:
		return "F7";
	case KEY_F08:
		return "F8";
	case KEY_F09:
		return "F9";
	case KEY_F10:
		return "F10";
	case KEY_F11:
		return "F11";
	case KEY_F12:
		return "F12";

	default: 
		return "undefined";
	}
}

//*/
#pragma endregion
