/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        Message.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       The base message class. Contains an enum for MessageTypes. Any new types of messages will need a new message type.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef MESSAGE_H
#define MESSAGE_H

enum MessageType
{
	MSG_NONE,
	MSG_KEYPRESS,
	MSG_MOUSEPRESS
};

namespace
{
	// Keep track of how many messages has been sent around since system start
	static int msgID = 0;
}

class Message
{
public:
	MessageType mMsgType;
	const int mID;

	Message(MessageType msgType) : mMsgType(msgType), mID(msgID++) {}
};


#endif
