#ifndef INPUT_MESSAGE_H
#define INPUT_MESSAGE_H
#include "Message.h"

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


#endif
