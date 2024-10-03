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
