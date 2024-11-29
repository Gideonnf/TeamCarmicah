/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ScriptFunctions.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:       Contains the static functions that are meant to be linked to mono c# so that c# scripts can call. Function Names have to match 
				the C# equivalent so that it can bind

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

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
#include "../Systems/SoundSystem.h"
#include "../ECS/SystemManager.h"
#include "../Systems/SceneSystem.h"

namespace Carmicah
{

	static std::unordered_map<MonoType*, std::function<bool(GameObject)>> mGameObjectHasComponentFuncs;

	// Define to make it easier to add internal function calls
#define ADD_INTERNAL_CALL(Name) mono_add_internal_call("Carmicah.FunctionCalls::" #Name, Name)

	/// <summary>
	/// Get the transform scale
	/// </summary>
	/// <param name="entityID"> ID of entity</param>
	/// <param name="outScale"> Scale of the entity</param>
	static void Transform_GetScale(unsigned int entityID, Vec2f* outScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		*outScale = go.GetComponent<Transform>().Scale();
	}

	/// <summary>
	/// Set the transform's scale
	/// </summary>
	/// <param name="entityID">ID of entity</param>
	/// <param name="inScale"> Take in a vec2f to set the scale with</param>
	static void Transform_SetScale(unsigned int entityID, Vec2f* inScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		go.GetComponent<Transform>().Scale(*inScale);
	}
	
	/// <summary>
	/// Check if an entity has a component, acts as the internal call to C# side so that C# can check what component it has
	/// Only checks against the components registered in RegisterComponents
	/// </summary>
	/// <param name="entityID">ID of entity</param>
	/// <param name="componentType">Type of component to check</param>
	/// <returns></returns>
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

	/// <summary>
	/// Fetches an entity based off their name by checking against registry in gGOFactory
	/// </summary>
	/// <param name="name"> Name of entity to find</param>
	/// <returns>ID of the entity</returns>
	static unsigned int Entity_FindEntityWithName(MonoString* name)
	{
		char* cStrName = mono_string_to_utf8(name);

		GameObject go{};
		gGOFactory->FetchGO(cStrName, go);

		mono_free(cStrName);

		return go.GetID();
	}

	/// <summary>
	/// Internal function call to play sound effects between C# and C++
	/// </summary>
	/// <param name="name">Name of the sound file to play</param>
	static void Sound_PlaySFX(MonoString* name, float volume)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySoundThis(cStrname, SoundCategory::SFX, SoundSystem::SOUND_INGAME, volume);
		mono_free(cStrname);
	}

	static void Sound_PlayBGM(MonoString* name, float volume)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySoundThis(cStrname, SoundCategory::BGM, SoundSystem::SOUND_BGM, volume);
		mono_free(cStrname);
	}

	static void Sound_Stop(MonoString* name)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSound(SoundSystem::SOUND_INGAME);
		mono_free(cStrname);
	}

	static void Sound_Pause(MonoString* name)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PauseSound(SoundSystem::SOUND_INGAME);
		mono_free(cStrname);
	}

	static void Sound_Resume(MonoString* name)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->ResumeSound(SoundSystem::SOUND_INGAME);
		mono_free(cStrname);
	}

	static void Sound_SetCategoryVolume(int category, float volume)
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->SetCategoryVolume(static_cast<SoundCategory>(category), SoundSystem::SOUND_INGAME, volume);
	}



	/// <summary>
	/// One way to apply linear force. Takes time into consideration
	/// </summary>
	/// <param name="entityID">ID of the entity</param>
	/// <param name="dir">Direction of the force</param>
	/// <param name="magnitude">Magnitude of the force</param>
	/// <param name="lifeTime">How long it will be applied for</param>
	static void RigidBody_ApplyForceWithTime(unsigned int entityID, Vec2f* dir, float magnitude, float lifeTime)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<RigidBody>())
		{
			LinearDirectionalForce dirForce( *dir , magnitude, lifeTime);

			go.GetComponent<RigidBody>().forcesManager.AddLinearForce(dirForce);
		}
		else
		{
			CM_CORE_ERROR("Entity does not have rigidBody");
		}
	}

	/// <summary>
	/// Another way to apply a force. Applies it as a single pulse where life time is 0
	/// </summary>
	/// <param name="entityID">ID of the entity</param>
	/// <param name="dir">Direction of the force</param>
	/// <param name="magnitude">Force of the magnitude</param>
	static void RigidBody_ApplyForce(unsigned int entityID, Vec2f dir, float magnitude)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<RigidBody>())
		{
			LinearDirectionalForce dirForce(dir, magnitude, 0.0f);

			go.GetComponent<RigidBody>().forcesManager.AddLinearForce(dirForce);
		}
		else
		{
			CM_CORE_ERROR("Entity does not have rigidBody");
		}

	}

	/// <summary>
	/// Get the position of the entity
	/// </summary>
	/// <param name="entityID">ID of the entity to check fr</param>
	/// <param name="outPos"> Out vector2 posiion</param>
	static void Transform_GetPosition(unsigned int entityID, Vec2f* outPos)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		*outPos = go.GetComponent<Transform>().Pos();
	}

	/// <summary>
	/// Set the position of hte transform
	/// </summary>
	/// <param name="entityID"> ID of the entity</param>
	/// <param name="inPos"> Vector2 pos to set with</param>
	static void Transform_SetPosition(unsigned int entityID, Vec2f* inPos)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		go.GetComponent<Transform>().Pos(*inPos);
	}

	/// <summary>
	/// Registers a component to C# mono side
	/// </summary>
	/// <typeparam name="T">Type of the component</typeparam>
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

	/// <summary>
	/// Interface for checking if key is pressed
	/// </summary>
	/// <param name="keyCode">Key code to check with</param>
	/// <returns>True or false</returns>
	static bool IsKeyPressed(Keys keyCode)
	{
		return Input.IsKeyPressed(keyCode);
	}

	/// <summary>
	/// Interface for checking if key hold
	/// </summary>
	/// <param name="keyCode">Key code to check with</param>
	/// <returns><Returns true or false/returns>
	static bool IsKeyHold(Keys keyCode)
	{
		return Input.IsKeyHold(keyCode);
	}

	/// <summary>
	/// Interface for changing the scene
	/// </summary>
	static void ChangeScene(MonoString* sceneName)
	{
		// char* that is allocated on the heap
		char* cStrName = mono_string_to_utf8(sceneName);
		// call the system manager to get instance of scene system
		auto& sceneSystem = SystemManager::GetInstance()->GetSystem<SceneSystem>();
		// change the scene
		sceneSystem->ChangeScene(cStrName);
		// free the memory
		mono_free(cStrName);
	}

	/// <summary>
	/// Register the component. Clear the map before registering
	/// </summary>
	void ScriptFunctions::RegisterComponents()
	{
		// if we hotload and need to rerun the linking and reinit mono
		// then we might need to clear the map before registering again
		mGameObjectHasComponentFuncs.clear();
		// Only these 2 for now
		RegisterComponent<Transform>();
		RegisterComponent<RigidBody>();
	}

	/// <summary>
	/// Register functions to C# side by using the internal mono call
	/// </summary>
	void ScriptFunctions::RegisterFunctions()
	{
		// Transform functions
		ADD_INTERNAL_CALL(Transform_GetScale);
		ADD_INTERNAL_CALL(Transform_SetScale);
		ADD_INTERNAL_CALL(Transform_GetPosition);
		ADD_INTERNAL_CALL(Transform_SetPosition);

		//Entity functions
		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(Entity_FindEntityWithName);

		// Rigidbody functions
		ADD_INTERNAL_CALL(RigidBody_ApplyForce);
		ADD_INTERNAL_CALL(RigidBody_ApplyForceWithTime);

		// input functions
		ADD_INTERNAL_CALL(IsKeyPressed);
		ADD_INTERNAL_CALL(IsKeyHold);

		// Button function
		ADD_INTERNAL_CALL(ChangeScene);

		// Sound
		ADD_INTERNAL_CALL(Sound_PlaySFX);
	}
}