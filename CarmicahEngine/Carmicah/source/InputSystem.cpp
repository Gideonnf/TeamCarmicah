#include "pch.h"
#include "./Systems/InputSystem.h"

#include <Windows.h> 


InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	// this function allows us to retrieve the entire sequence of states of each key of the keyboard
	// to do that, the function needs a pointer to an array of 256 unsigned char byte values. this pointer is m_keys_state
	if (::GetKeyboardState(m_keys_state))
	{
		// need to check the state of each key one by one
		for (unsigned int i{}; i < 256; i++)
		{
			// check the state of the key
			// this is called bitmask nd the higher order bits of the value is evaluated
			// if this value is 1 or 2, the key is down. otherwise, not
			if (m_keys_state[i] & 0x80) // KEY IS DOWN
			{
				// if key is down, need to loop our maplisteners and notify each listener that the "i" key is pressed by calling on key down

				std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.begin();

				while (it != m_map_listeners.end())
				{
					it->second->onKeyDown(i);
					++it;
				}
			}
			else // KEY IS UP
			{
				// key is up and released but we dont know the precise moment that it is released
				// solution to this is to store all the keys state after the for loop in another array
				// at the next update call, check if the key is UP if the old state of the key is not equal to the current one
				// and if this is the case, we have a key release EVENT
				// then we can loop through the listeners and inform them of a UP KEY event
				std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.begin();

				while (it != m_map_listeners.end())
				{
					it->second->onKeyUp(i);
					++it;
				}
			}
		}
		// store current keys state to old keys state buffer
		::memcpy(m_old_keys_state, m_keys_state, sizeof(unsigned char) * 256);
	}

}

void InputSystem::addListener(InputListener* listener)
{
	// add the listener pointer to the map by using insert
	m_map_listeners.insert(std::make_pair<InputListener*, InputListener*>
		(std::forward<InputListener*>(listener), std::forward<InputListener*>(listener)));
}

void InputSystem::removeListener(InputListener* listener)
{
	// find the listener parsed as a param in the map
	std::map<InputListener*, InputListener*>::iterator it = m_map_listeners.find(listener);

	// if it is found, remove it. Otherwise, nothing happens
	if (it != m_map_listeners.end())
	{
		m_map_listeners.erase(it);
	}
}

InputSystem* InputSystem::get()
{
	// return address of static inputsystem object
	static InputSystem system;
	return nullptr;
}
