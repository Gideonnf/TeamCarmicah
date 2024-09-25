#include  "pch.h"
#pragma region C Rendition of Events.cpp
/*


#include "../include/Systems/Events.h"
#include "pch.h"
#include <stdlib.h>


#define MAX_MESSAGE_CODES 100


//-------------------------------------------------------------------------------------------------
#pragma region global structs

//-------------------------------------------------------------------------------------------------
// global struct to create track of registered events
typedef struct Subscribed_Event
{
	// will have subscriber
	void* subscriber;
	// calback
	PFN_on_event callback;
}Subscribed_Event;


//-------------------------------------------------------------------------------------------------
// this will
typedef struct EventCode_Entry
{
	//Subscribed_Event* events;
	// create vector rather than pointer/ raw array
	std::vector<Subscribed_Event> events;
	size_t count;
	size_t capacity;

}EventCode_Entry;


//-------------------------------------------------------------------------------------------------
// struct for the event system state, for the event itself
typedef struct EventSystem_State
{
	EventCode_Entry subscribed[MAX_MESSAGE_CODES];
}EventSystem_State;

#pragma endregion


//-------------------------------------------------------------------------------------------------
static EventSystem_State* iState;
typedef bool (*PFN_on_event)(u16 code, void* sender, void* listener, EventData eventdata);


//-------------------------------------------------------------------------------------------------
#pragma region event_system_initialise
void event_system_initialise(void)
{
	// assign mem
	if (!iState)
	{
		iState = new EventSystem_State();
	}

	// check if it failed to allocate mem
	if (!iState)
	{
		std::cout << "Failed to allocate memory for EventSystem_State" << std::endl;
		exit(EXIT_FAILURE);
	}

	// if it gets here, mem has been allocated, initialise the eventsystem officially
	for (int i{}; i < MAX_MESSAGE_CODES; ++i)
	{
		// clear everything
		iState->subscribed[i].events.clear();		
	}
}

#pragma endregion


//-------------------------------------------------------------------------------------------------
#pragma region event_system_shutdown
// Free the memory taken by eventstate_system
void event_system_shutdown(void)
{
	if (iState)
	{
		// loop through all events
		for (int i{}; i < MAX_MESSAGE_CODES; ++i)
		{
			if (iState->subscribed[i].events)
			{
				// deallocate the memory
				free(iState->subscribed[i].events);
				iState->subscribed[i].events = NULL;
			}
		}
		free(iState);
		iState = NULL;
	}
}

#pragma endregion


//-------------------------------------------------------------------------------------------------
#pragma region resize_event_array
// i dont really need this because my vector of events is dynamic and expendable
// but better safe than sorry?
bool resize_event_array(EventCode_Entry* entry)
{
	size_t new_capacity = (entry->capacity == 0) ? 1:(entry->capacity * 2);
}

#pragma endregion



//-------------------------------------------------------------------------------------------------
#pragma region event_subscribe
bool event_subscribe(u16 code, void* subscriber, PFN_on_event on_event)
{
	// check if iState exists, if not, boot us out
	if (!iState)
	{
		return false;
	}

	// create an entry
	// this is saying: I want to create a EVENT_CODE_KEY_PRESSED entry
	EventCode_Entry* entry = &iState->subscribed[code]; 

	// check if there's enough capacity for new entry
	if (entry->count >= entry->capacity)
	{

	}

	// the subscriber can subscribe to the event code but u dont want to do it multiple times, below is the check
	// that it doesnt do more than once and that is alr registered to a code
	for (size_t i{}; i < entry->count; ++i)
	{
		if (entry->events[i].subscriber == subscriber && entry->events[i].callback == on_event)
		{
			std::cout << "Event has already been registered" << std::endl;
			return false;
		}
	}

	// If it gets here, its new and unregistered. Now we add the new event
	//entry->events[entry->count].subscriber	= subscriber;
	//entry->events[entry->count].callback	= on_event;
	entry->events.push_back({ subscriber, on_event });
	entry->count++;
	return true;

}
#pragma endregion


//-------------------------------------------------------------------------------------------------
#pragma region event_unsubscribe
void event_unsubscribe(u16 code, void* subscriber, PFN_on_event on_event)
{
	// sanity check
	if (!iState)
	{
		return;
	}

	EventCode_Entry* entry = &iState->subscribed[code];

	if (!entry->events)
	{
		return;
	}

	for (size_t i{}; i < entry->count; ++i)
	{
		if (entry->events[i].subscriber == subscriber && entry->events[i].callback == on_event)
		{
			for (size_t j{ i }; j < entry->count; ++j)
			{
				entry->events[j] = entry->events[j + 1];
			}
			entry->count--;
		}
	}
}

#pragma endregion


//-------------------------------------------------------------------------------------------------
#pragma region event_publish
bool event_publish(u16 code, void* subscriber, EventData eventdata)
{
	if (!iState)
	{
		return false;
	}

	// if nothing is registered for the code, do nothing
	if (!iState->subscribed[code].events)
	{
		return false;
	}

	EventCode_Entry* entry = &iState->subscribed[code];

	for (int i{}; i < entry->count; ++i)
	{
		Subscribed_Event e = entry->events[i];
		if (e.callback(code, subscriber, e.subscriber, eventdata))
		{
			// message has been handled, we dont have to send to other listeners/subscribers
			break;
		}
	}
	return true; 
}

#pragma endregion


*/
#pragma endregion




#pragma region C++ Version of Event.cpp
///*

#include "Systems/Events.h"


void EventSystem::initialise() 
{
	// No manual memory management needed, just clear the events
	for (auto& entry : subscribed) 
	{
		entry.events.clear();
	}
}

void EventSystem::shutdown() 
{
	// Clear events
	for (auto& entry : subscribed) 
	{
		entry.events.clear();
	}
}

bool EventSystem::subscribe(u16 code, void* subscriber, PFN_on_event on_event) 
{
	// Bounds check
	if (code >= 100) 
	{
		std::cerr << "Event code out of bounds" << std::endl;
		return false;
	}

	EventCodeEntry& entry = subscribed[code];

	// Prevent double subscription
	for (const auto& event : entry.events) 
	{
		if (event.subscriber == subscriber /*&& event.callback == on_event*/) 
		{
			std::cout << "Event already registered" << std::endl;
			return false;
		}
	}

	// Add new event
	entry.events.push_back({ subscriber, on_event });
	return true;
}

void EventSystem::unsubscribe(u16 code, void* subscriber, PFN_on_event on_event) {
	if (code >= 100) return;

	EventCodeEntry& entry = subscribed[code];
	for (auto it = entry.events.begin(); it != entry.events.end(); ++it) 
	{
		if (it->subscriber == subscriber /*&& it->callback == on_event*/) {
			entry.events.erase(it);
			break;
		}
	}
}

bool EventSystem::publish(u16 code, void* subscriber, EventData eventdata) 
{
	if (code >= 100) return false;

	EventCodeEntry& entry = subscribed[code];
	for (auto& event : entry.events) {
		if (event.callback(code, subscriber, event.subscriber, eventdata)) 
		{
			// Message handled, no need to continue
			break;
		}
	}
	return true;
}


//*/
#pragma endregion