#pragma once

#include <stdio.h>
#include "Systems/InputTypes.h"
#include "Systems/Defines.h"


bool input_system_initialise();
void input_system_shutdown();

bool input_is_key_down(Keys key);
bool input_is_key_up(Keys key);

bool input_was_key_down(Keys key);
bool input_was_key_up(Keys key);

void input_process_keys(Keys key, bool pressed);

bool input_is_mouse_down(Mouse_Buttons button);
bool input_is_mouse_up(Mouse_Buttons button);

bool input_was_mouse_down(Mouse_Buttons button);
bool input_was_mouse_up(Mouse_Buttons button);

bool input_is_mouse_button_dragging(Mouse_Buttons button);

void input_get_mouse_position(i16* x, i16* y); // return the mouse position
void input_get_previous_mouse_position(i16* x, i16* y); 

void input_process_mouse_button(Mouse_Buttons button, bool pressed);
void input_process_mouse_move(i16 x, i16 y);

// idk if i need this but i'll follow the tutorial because maybe we want the scrolling
// to be done by wheel scrolling
void input_process_mouse_wheel(i8 delta_z);

// this parses and returns what our keys map to, it returns the name of the specific key
const char* input_keycode_to_string(Keys key);