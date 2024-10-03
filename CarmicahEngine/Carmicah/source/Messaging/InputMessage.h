/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        InputMessage.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       An example of how to create messages that can be sent between systems. Inherit a base message class and send in the type of message as an enum

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef INPUT_MESSAGE_H
#define INPUT_MESSAGE_H
#include "Message.h"

namespace Carmicah
{
	class KeyMessage : public Message
	{
	public:
		int mKeypress;
		KeyMessage( int key) : Message(MSG_KEYPRESS), mKeypress(key) {}
	};

	class MouseMessage : public Message
	{
	public:
		int mMouseKey;
		MouseMessage( int key) : Message(MSG_MOUSEPRESS), mMouseKey(key) {}
	};
}


#endif
