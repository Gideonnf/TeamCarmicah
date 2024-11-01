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
#include "../ECS/ECSTypes.h"

namespace Carmicah
{
	enum MessageType
	{
		MSG_NONE,
		MSG_KEYPRESS,
		MSG_MOUSEPRESS,
		MSG_ENTITYKILLED,
		MSG_UPDATETRANSFORM
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
		virtual ~Message() {}
	};

	class EntityKilledMessage : public Message
	{
	public:
		Entity mEntityID;
		EntityKilledMessage(Entity id) : Message(MSG_ENTITYKILLED), mEntityID(id) {}
	};

	class UpdateTransformMessage : public Message
	{
	public:
		Entity mEntityID;
		Entity mParentID;
		UpdateTransformMessage(Entity id, Entity parentID) : Message(MSG_UPDATETRANSFORM), mEntityID(id), mParentID(parentID) {}
	};


}

#endif
