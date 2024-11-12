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
#include "../Systems/AssetTypes.h"

namespace Carmicah
{
	enum MessageType
	{
		MSG_NONE,
		MSG_KEYPRESS,
		MSG_MOUSEPRESS,
		MSG_ENTITYKILLED,
		MSG_UPDATETRANSFORM,
        MSG_ENTITYPICKED,
		MSG_RUNTIME,
		MSG_EDITORENTITY,
		MSG_BUTTONENTITY,
		MSG_PLAYBGM,
		MSG_PLAYSFX,
		MSG_MODIFYPREFAB
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

	class EntityPickedMessage : public Message
	{
	public:
		Entity mEntityID;
		EntityPickedMessage(Entity id) : Message(MSG_ENTITYPICKED), mEntityID(id) {}
	};

	class EditorEntityPicked : public Message
	{
	public:
		Entity mEntityID;
		EditorEntityPicked(Entity id) : Message(MSG_EDITORENTITY), mEntityID(id) {}

	};

	class ButtonClicked : public Message
	{
	public:
		Entity mEntityID;
		ButtonClicked(Entity id) : Message(MSG_BUTTONENTITY), mEntityID(id) {}
	};

	class RuntimeStartMessage : public Message
	{
	public:
		//bool mRuntime;
		RuntimeStartMessage() : Message(MSG_RUNTIME) {}
	};

	class PlayBGMMsg : public Message
	{
	public:
		std::string fileName;
		PlayBGMMsg(std::string file) : Message(MSG_PLAYBGM), fileName(file) {}
	};

	class PlaySFXMsg : public Message
	{
	public:
		std::string fileName;
		PlaySFXMsg(std::string file) : Message(MSG_PLAYSFX), fileName(file) {}
	};

	class ModifyPrefabMsg : public Message
	{
	public:
		Prefab& prefabRef;
		ModifyPrefabMsg(Prefab& prefab) : Message(MSG_MODIFYPREFAB), prefabRef(prefab) {}
	};
}

#endif
