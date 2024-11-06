#include <pch.h>
#include "ScriptFunctions.h"
#include "../Math/Vec2.h"
#include <mono/metadata/object.h>
#include <mono/metadata/reflection.h>
#include "../Systems/GOFactory.h"
#include "../ECS/EntityManager.h"
#include "../ECS/ComponentManager.h"
#include "../Components/Transform.h"
#include "../Components/RigidBody.h"
#include "ScriptSystem.h"
#include "../Input/InputSystem.h"

namespace Carmicah
{

	static std::unordered_map<MonoType*, std::function<bool(GameObject)>> mGameObjectHasComponentFuncs;
#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("Carmicah.FunctionCalls::" #Name, Name)

	static void Transform_GetScale(unsigned int entityID, Vec2f* outScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		*outScale = go.GetComponent<Transform>().scale;
	}

	static void Transform_SetScale(unsigned int entityID, Vec2f* inScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		go.GetComponent<Transform>().scale = *inScale;
	}

	static bool Entity_HasComponent(unsigned int entityID, MonoReflectionType* componentType)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		MonoType* monoType = mono_reflection_type_get_type(componentType);

		if (mGameObjectHasComponentFuncs.count(monoType) <= 0)
		{
			// component not registered
			CM_CORE_ERROR("Component Not Registered");
			assert("Component not registered");
		}
		
		return mGameObjectHasComponentFuncs[monoType](go);
	}

	static void RigidBody_ApplyForceWithTime(unsigned int entityID, Vec2f* dir, float magnitude, float lifeTime)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<RigidBody>())
		{
			LinearDirectionalForce dirForce( *dir , magnitude, lifeTime, false);

			go.GetComponent<RigidBody>().forcesManager.AddLinearForce(dirForce);
		}
		else
		{
			CM_CORE_ERROR("Entity does not have rigidBody");
		}
	}

	static void RigidBody_ApplyForce(unsigned int entityID, Vec2f dir, float magnitude)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<RigidBody>())
		{
			LinearDirectionalForce dirForce(dir, magnitude, 1.0, false);

			go.GetComponent<RigidBody>().forcesManager.AddLinearForce(dirForce);
		}
		else
		{
			CM_CORE_ERROR("Entity does not have rigidBody");
		}

	}

	template <typename T>
	static void RegisterComponent()
	{
		std::string_view typeName = typeid(T).name();
		size_t pos = typeName.find_last_of(':');
		std::string_view structName = typeName.substr(pos + 1);
		// so that we can match the C# equivalent of the component
		std::string modifiedTypename = fmt::format("Carmicah.{}", structName);

		MonoType* monoType = mono_reflection_type_from_name(modifiedTypename.data(), gScriptSystem->mCoreAssemblyImage);
		if (!monoType)
		{
			CM_CORE_ERROR("Couldn't find component");
			assert("Can't find component");
			return;
		}
		mGameObjectHasComponentFuncs[monoType] = [](GameObject go) { return go.HasComponent<T>();  };

	}

	static bool IsKeyPressed(Keys keyCode)
	{
		return Input.IsKeyPressed(keyCode);
	}

	void ScriptFunctions::RegisterComponents()
	{
		// if we hotload and need to rerun the linking and reinit mono
		// then we might need to clear the map before registering again
		mGameObjectHasComponentFuncs.clear();
		// Only these 2 for now
		RegisterComponent<Transform>();
		RegisterComponent<RigidBody>();
	}

	void ScriptFunctions::RegisterFunctions()
	{
		// Transform functions
		ADD_INTERNAL_CALL(Transform_GetScale);
		ADD_INTERNAL_CALL(Transform_SetScale);

		//Entity functions
		ADD_INTERNAL_CALL(Entity_HasComponent);

		// Rigidbody functions
		ADD_INTERNAL_CALL(RigidBody_ApplyForce);
		ADD_INTERNAL_CALL(RigidBody_ApplyForceWithTime);

		ADD_INTERNAL_CALL(IsKeyPressed);
	}
}