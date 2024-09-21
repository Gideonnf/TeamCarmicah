#include "pch.h"
#include "Systems/InputSystem.h"
#include "Messaging/InputMessage.h"
namespace Carmicah
{
	void InputSystem::Init()
	{
		int keyCode = 5;
		KeyMessage msg(keyCode);
		SendMessage(&msg);
	}
}