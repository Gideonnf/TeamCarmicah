#pragma region C Rendition of Event.h
/*

#pragma once


#include <stdio.h>
#include "Systems/Defines.h"

typedef enum EventCode : u16
{
	EVENT_CODE_KEY_PRESSED = 0,
	EVENT_CODE_KEY_RELEASED,
	EVENT_CODE_MOUSE_BUTTON_PRESSED,
	EVENT_CODE_MOUSE_BUTTON_CLICKED,
	EVENT_CODE_MOUSE_BUTTON_RELEASED,
	EVENT_CODE_MOUSE_DRAGGED,
	EVENT_CODE_MOUSE_DRAG_BEGIN,
	EVENT_CODE_MOUSE_DRAG_END,
	EVENT_CODE_MOUSE_WHEEL,
	EVENT_CODE_MOUSE_MOVED,
} EventCode;

typedef struct EventData
{
	union 
	{
		i64 i64[2];
		u64 u64[2];
		f32 f32[2];

		i32 i32[4];
		u32 u32_array[4];
		
		i16 i16[8];
		u16 u16[8];

		i8 i8[16];
		u8 u8[16];

		union
		{
			u32 size; // why tf doesnt this init properly, it is literally right there above 
			void* data;
		}custom_data;

		const char* s;
	}data;
}EventData;

void event_system_initialise(void);
void event_system_shutdown(void);

typedef bool (*PFN_on_event)(u16 code, void* sender, void* listener, EventData eventdata);

// event code, subscriber can be any function, event callback
bool event_subscribe(u16 code, void* subscriber, PFN_on_event on_event); 
void event_unsubscribe(u16 code, void* subscriber, PFN_on_event on_event);

bool event_publish(u16 code, void* subscriber, EventData eventdata);


*/
#pragma endregion


#pragma region C++ Rendition of Event.h
///*


#pragma once

#include <iostream>
#include <functional>
#include <vector>
#include "Systems/Defines.h"

enum class EventCode : u16 {
    EVENT_CODE_KEY_PRESSED = 0,
    EVENT_CODE_KEY_RELEASED,
    EVENT_CODE_MOUSE_BUTTON_PRESSED,
    EVENT_CODE_MOUSE_BUTTON_CLICKED,
    EVENT_CODE_MOUSE_BUTTON_RELEASED,
    EVENT_CODE_MOUSE_DRAGGED,
    EVENT_CODE_MOUSE_DRAG_BEGIN,
    EVENT_CODE_MOUSE_DRAG_END,
    EVENT_CODE_MOUSE_WHEEL,
    EVENT_CODE_MOUSE_MOVED,
};

struct EventData {
    union {
        i64 i64[2];
        u64 u64[2];
        f32 f32[2];
        i32 i32[4];
        u32 u32_array[4]; // fucking have to rename this cus the custom data cannot have same name bitch ass nigger
        i16 i16[8];
        u16 u16[8];
        i8 i8[16];
        u8 u8[16];

        struct {
            u32 size;
            void* data;
        } custom_data;

        const char* s;
    } data;
};

using PFN_on_event = std::function<bool(u16, void*, void*, EventData)>;

class EventSystem {
public:
    void initialise();
    void shutdown();

    bool subscribe(u16 code, void* subscriber, PFN_on_event on_event);
    void unsubscribe(u16 code, void* subscriber, PFN_on_event on_event);
    bool publish(u16 code, void* subscriber, EventData eventdata);

private:
    struct SubscribedEvent {
        void* subscriber;
        PFN_on_event callback;
    };

    struct EventCodeEntry {
        std::vector<SubscribedEvent> events;
    };

    EventCodeEntry subscribed[100];  // Fixed array of event code entries
};


//*/
#pragma endregion