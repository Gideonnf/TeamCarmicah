#ifndef MOUSE_SYSTEM_H
#define MOUSE_SYSTEM_H
#include "ECS/BaseSystem.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"

namespace Carmicah
{
	class MouseSystem : public BaseSystem
	{
	public:
		void Init();
		void Update();
		void Exit();

		void ReceiveMessage(Message* msg) override;
		void OnEnter(Entity entityID);
		void OnExit(Entity entityID);
		void OnHover(Entity entityID);
	};
}

#endif