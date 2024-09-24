#pragma once
#include "InputListener.h"
#include <map>

class InputSystem
{
public:
	InputSystem();	// default ctor
	~InputSystem(); // default dtor

	void update(); // here we get the input events from our devices
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);


public:
	static InputSystem* get(); // return a pointer to a only instance of this class

private:
	// in order to add listeners to out input system, we need a container
	std::map<InputListener*, InputListener*> m_map_listeners;
	unsigned char m_keys_state[256] = {};
	unsigned char m_old_keys_state[256] = {};

};