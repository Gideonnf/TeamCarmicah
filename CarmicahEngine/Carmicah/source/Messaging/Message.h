/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:			Message.h

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

 brief:			The base message class. Contains an enum for MessageTypes. Any new types of messages will need a new message type.

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
		MSG_ENTITYCOLLIDED,
		MSG_RUNTIME,
		MSG_EDITORENTITY,
		MSG_BUTTONENTITY,
		MSG_PLAYBGM,
		MSG_PLAYSFX,
		MSG_NEWPREFABGO,
		MSG_MODIFYPREFAB,
		MSG_NEWPREFAB,
		MSG_ONCLICK,
		MSG_UPDATEHIERARCHY,
		MSG_MOUSEENTER,
		MSG_MOUSEHOVER,
		MSG_MOUSEEXIT,
		MSG_ENTERSTATE,
		MSG_UPDATESTATE,
		MSG_EXITSTATE
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

	class UpdateHierarchyMessage : public Message
	{
	public:
		Entity mEntityID;
		Entity mParentID;
		UpdateHierarchyMessage(Entity id, Entity parentID) : Message(MSG_UPDATEHIERARCHY), mEntityID(id), mParentID(parentID) {}
	};
	
	class EntityPickedMessage : public Message
	{
	public:
		Entity mEntityID;
		EntityPickedMessage(Entity id) : Message(MSG_ENTITYPICKED), mEntityID(id) {}
	};

	class EntityCollidedMessage : public Message
	{
	public:
		Entity mEntityID;
		EntityCollidedMessage(Entity id) : Message(MSG_ENTITYCOLLIDED), mEntityID(id) {}
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

	class NewPrefabGOMsg : public Message
	{
	public:
		Entity entityID;
		unsigned int prefabID;
		NewPrefabGOMsg(Entity id, unsigned int pId) : Message(MSG_NEWPREFABGO), entityID(id), prefabID(pId) {}
	};

	class NewPrefabMsg : public Message
	{
	public:
		Entity goEntity;
		NewPrefabMsg(Entity go) : Message(MSG_NEWPREFAB), goEntity(go) {}
	};

	class OnClickMsg : public Message
	{
	public:
		Entity buttonEntity;
		OnClickMsg(Entity go) : Message(MSG_ONCLICK), buttonEntity(go) {}
	};

	class OnMouseMsg : public Message
	{
	public:
		Entity entity;
		OnMouseMsg(MessageType type, Entity go) : Message(type), entity(go) {}
	};

	class OnStateEnterMsg : public Message
	{
	public:
		Entity entity;
		std::string stateName;

		OnStateEnterMsg(Entity go, std::string state) : Message(MSG_ENTERSTATE), entity(go), stateName(state) {}
	};

	class OnStateUpdateMsg : public Message
	{
	public:
		Entity entity;
		std::string stateName;
		float dt;

		OnStateUpdateMsg(Entity go, std::string state, float delta) : Message(MSG_UPDATESTATE), entity(go), stateName(state), dt(delta) {}
	};

	class OnStateExitMsg : public Message
	{
	public:
		Entity entity;
		std::string stateName;

		OnStateExitMsg(Entity go, std::string state) : Message(MSG_EXITSTATE), entity(go), stateName(state) {}
	};
}

#endif
