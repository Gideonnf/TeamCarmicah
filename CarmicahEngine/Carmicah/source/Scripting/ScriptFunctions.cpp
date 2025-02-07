/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ScriptFunctions.cpp

 author:		Gideon Francis (100%)

 email:			g.francis@digipen.edu

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
#include "../Components/Animation.h"
#include "ScriptSystem.h"
#include "../Input/InputSystem.h"
#include "../Systems/SoundSystem.h"
#include "../ECS/SystemManager.h"
#include "../Systems/SceneSystem.h"
#include "../Editor/SceneWindow.h"
#include "../FSM/FSMSystem.h"

namespace Carmicah
{
	std::string MonoToString(MonoString* monoStr)
	{
		if (!monoStr) return "";

		char* utf8Str = mono_string_to_utf8(monoStr);
		if (!utf8Str) return "";

		std::string result(utf8Str);
		mono_free(utf8Str);

		return result;
	}

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

		/*if (go.GetName().find("Bullet") != std::string::npos && go.GetName() != "Bullet")
		{
			if (go.HasComponent<RigidBody>())
				std::cout << "delet later";
		}*/

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
		std::string cStrName = MonoToString(name);

		GameObject go{};
		gGOFactory->FetchGO(cStrName, go);

		//mono_free(cStrName);

		return go.GetID();
	}

	static unsigned int Entity_FindEntityWithID(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		return go.GetID();
	}

	static unsigned int Entity_GetParent(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
		{
			return go.GetComponent<Transform>().parent;
		}
		else if (go.HasComponent<UITransform>())
		{
			return go.GetComponent<UITransform>().parent;
		}

		return 0;
	}

	/// <summary>SS
	/// Internal function call to play sound effects between C# and C++
	/// </summary>
	/// <param name="name">Name of the sound file to play</param>
	static void Sound_PlaySFX(MonoString* name, float volume)
	{
		std::string cStrName = MonoToString(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySoundThis(cStrName, SoundCategory::SFX, SoundSystem::SOUND_INGAME, false, volume);
		//mono_free(cStrname);
	}

	static void Sound_PlayBGM(MonoString* name, float volume)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySoundThis(cStrname, SoundCategory::BGM, SoundSystem::SOUND_BGM, true, volume);
		mono_free(cStrname);
	}

	static void Sound_StopBGM(MonoString* name)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSound(SoundSystem::SOUND_BGM);
		mono_free(cStrname);
	}

	//static void Sound_Stop(MonoString* name)
	//{
	//	char* cStrname = mono_string_to_utf8(name);
	//	auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
	//	souSystem->StopSound(SoundSystem::SOUND_INGAME);
	//	mono_free(cStrname);
	//}

	//static void Sound_Pause(MonoString* name)
	//{
	//	char* cStrname = mono_string_to_utf8(name);
	//	auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
	//	souSystem->PauseSound(SoundSystem::SOUND_INGAME);
	//	mono_free(cStrname);
	//}

	//static void Sound_Resume(MonoString* name)
	//{
	//	char* cStrname = mono_string_to_utf8(name);
	//	auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
	//	souSystem->ResumeSound(SoundSystem::SOUND_INGAME);
	//	mono_free(cStrname);
	//}

	//static void Sound_SetCategoryVolume(int category, float volume)
	//{
	//	auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
	//	souSystem->SetCategoryVolume(static_cast<SoundCategory>(category), SoundSystem::SOUND_INGAME, volume);
	//}



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

	static void Transform_GetLocalPosition(unsigned int entityID, Vec2f* outPos)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		if (go.HasComponent<Transform>())
		{
			*outPos = go.GetComponent<Transform>().Pos();

			//
		}
		else if (go.HasComponent<UITransform>())
		{
			*outPos = go.GetComponent<UITransform>().Pos();
		}
		else
		{
			*outPos = { 0, 0 };
		}
	}

	/// <summary>
	/// Get the position of the entity
	/// </summary>
	/// <param name="entityID">ID of the entity to check fr</param>
	/// <param name="outPos"> Out vector2 posiion</param>
	static void Transform_GetPosition(unsigned int entityID, Vec2f* outPos)
	{
		//CM_CORE_INFO("Entity ID scriptFunctions {}", entityID);
		// idk why tf does it sometimes give a extremely high entity ID number
		GameObject& go = gGOFactory->FetchGO(entityID);

		//CM_CORE_INFO("Entity ID scriptFunctions {}", go.GetID());

		if (go.HasComponent<Transform>())
		{
			*outPos = go.GetComponent<Transform>().Pos();
			// If it has a parent
			if (go.GetComponent<Transform>().parent != 0)
			{
				Transform parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(go.GetComponent<Transform>().parent);
				*outPos += parentTransform.Pos();

			}
		}
		else if (go.HasComponent<UITransform>())
		{
			
			*outPos = go.GetComponent<UITransform>().Pos();

		/*	if (entityID == 29)
				CM_CORE_INFO("{}, {}", outPos->x, outPos->y);*/
		}
		else
		{
			*outPos = { 0 , 0 };
		}
	}

	/// <summary>
	/// Set the position of hte transform
	/// </summary>
	/// <param name="entityID"> ID of the entity</param>
	/// <param name="inPos"> Vector2 pos to set with</param>
	static void Transform_SetPosition(unsigned int entityID, Vec2f* inPos)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
			go.GetComponent<Transform>().Pos(*inPos);
		else if (go.HasComponent<UITransform>())
			go.GetComponent<UITransform>().Pos(*inPos);
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
		//if (keyCode == Keys::KEY_ESCAPE)
		//{
		//	if (Input.IsKeyPressed(keyCode))
		//	{
		//		CM_CORE_INFO("ESC KEY BEING PRESSED");

		//	}
		//	else
		//	{
		//		CM_CORE_INFO("ESC KEY NOT BEING PRESSED");
		//	}
		//}
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

	static bool IsMousePressed(MouseButtons button)
	{
		return Input.IsMousePressed(button);
	}

	static bool IsMouseReleased(MouseButtons button)
	{
		//if (Input.IsMouseReleased(button))
		//{
		//	CM_CORE_INFO("LEFT MOUSE BUTTON RELEASE");
		//}
		return Input.IsMouseReleased(button);
	}

	/// <summary>
	/// Interface for changing the scene
	/// </summary>
	static bool ChangeScene(MonoString* sceneName)
	{
		// char* that is allocated on the heap
		std::string cStrName = MonoToString(sceneName);
		// call the system manager to get instance of scene system
		auto sceneSystem = SystemManager::GetInstance()->GetSystem<SceneSystem>();
		// change the scene
		if (!sceneSystem->ChangeScene(cStrName))
		{
			//mono_free(cStrName);
			return false;
		}
		// free the memory
		//mono_free(cStrName);
		return true;
	}

	static void Destroy(unsigned int entityID)
	{
		CM_CORE_INFO("Destroying entity {}", entityID);
		gGOFactory->Destroy(entityID);
	}

	static unsigned int CreateNewGameObject(MonoString* prefabName)
	{
		std::string cStrName = MonoToString(prefabName);
		GameObject newGO = gGOFactory->CreatePrefab(cStrName);
		//mono_free(cStrName);

		return newGO.GetID();
	}

	static MonoObject* GetScriptInstance(unsigned int entityID)
	{
		if (gScriptSystem->mEntityInstances.count(entityID) > 0)
		{
			return gScriptSystem->mEntityInstances[entityID]->GetInstance();
		}
		else
		{
			CM_CORE_ERROR("Entity does not have script attached");
			return nullptr;
		}
	}

	static MonoObject* GetScriptInstanceFromChildren(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		if (go.HasComponent<Transform>())
		{
			if (go.GetComponent<Transform>().children.size() > 0)
			{
				for each (Entity child in go.GetComponent<Transform>().children)
				{
					if (gScriptSystem->mEntityInstances.count(child))
					{
						return gScriptSystem->mEntityInstances[child]->GetInstance();
					}
				}
			}
		}
		else if (go.HasComponent<UITransform>())
		{
			if (go.GetComponent<UITransform>().children.size() > 0)
			{
				for each (Entity child in go.GetComponent<UITransform>().children)
				{
					if (gScriptSystem->mEntityInstances.count(child))
					{
						return gScriptSystem->mEntityInstances[child]->GetInstance();
					}
				}
			}
		}

		/*if (gScriptSystem->mEntityInstances.count(entityID) > 0)
		{
			return gScriptSystem->mEntityInstances[entityID]->GetInstance();
		}
		else
		{
			CM_CORE_ERROR("Entity does not have script attached");
			return nullptr;
		}*/
	}

	static void Transform_GetDepth(unsigned int entityID, float* outFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (!go.HasComponent<Transform>())
			*outFloat = 0.0f;
		else
			*outFloat = go.GetComponent<Transform>().GetDepth();
	}

	static void Transform_SetDepth(unsigned int entityID, float* inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
			go.GetComponent<Transform>().Depth(*inFloat);
	}

	static MonoString* Transform_GetTag(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
		{
			return mono_string_new(mono_domain_get(), go.GetComponent<Transform>().transformTag.c_str());
		}
		else if (go.HasComponent<UITransform>())
		{
			return mono_string_new(mono_domain_get(), go.GetComponent<UITransform>().transformTag.c_str());

		}
	}

	static void Animation_ChangeAnim(unsigned int entityID, MonoString* string)
	{
		std::string cStrName = MonoToString(string);
		GameObject& go = gGOFactory->FetchGO(entityID);
		//CM_CORE_INFO("Entity ID in changeAnim: {}", entityID);
		//go.GetComponent<Animation>().animAtlas = cStr;
		go.GetComponent<Animation>().ChangeAnim(cStrName);
		//mono_free(cStr);
	}

	static float GetMaxTime(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Animation>())
		{
			AnimAtlas& a{ AssetManager::GetInstance()->GetAsset<AnimAtlas>(go.GetComponent<Animation>().GetAnimAtlas()) };
			float time = 0.0f;
			for each (auto iter in a.anim)
			{
				time += iter.first;
			}

			return time;
		}
	}

	static void SetStateCondition(unsigned int entityID, MonoObject* obj)
	{
		if (!obj) return;

		MonoClass* objClass = mono_object_get_class(obj);
		const char* className = mono_class_get_name(objClass);
		variantVar var;
		// its an int obj
		if (strcmp(className, "Int32") == 0)
		{
			int* value = (int*)mono_object_unbox(obj);
			var = *value;
			//std::cout << "Received int: " << *value << std::endl;
		}
		else if (strcmp(className, "Single") == 0)
		{
			float* value = (float*)mono_object_unbox(obj);
			var = *value;
			//std::cout << "Received float: " << *value << std::endl;
		}
		else if (strcmp(className, "Boolean") == 0)
		{
			bool* value = (bool*)mono_object_unbox(obj);
			var = *value;
			//std::cout << "Received bool: " << *value << std::endl;
		}
		else if (strcmp(className, "String") == 0)
		{
			MonoString* monoStr = (MonoString*)obj;
			std::string cStrName = MonoToString(monoStr);
			var = cStrName;
			//std::cout << "Received string: " << utf8Str << std::endl;
			//mono_free(utf8Str); // Free Mono allocated memory
		}
		else
		{
			std::cout << "Unknown object type: " << className << std::endl;
		}
	
		auto& fsmSystem = SystemManager::GetInstance()->GetSystem<FSMSystem>();
	
		fsmSystem->SetCondition(entityID, var);
	}

	static float GetStateTimer(unsigned int entityID)
	{
		auto& fsmSystem = SystemManager::GetInstance()->GetSystem<FSMSystem>();

		return fsmSystem->GetStateTimer(entityID);
	}

	static void CloseGame()
	{
		//glfwSetWindowShouldClose(window, GL_TRUE);
#ifdef CM_INSTALLER
		Input.CloseGame();
		return;
#endif

#ifndef CM_INSTALLER
		SceneWindow::mIsPlaying = !SceneWindow::mIsPlaying;
		SceneWindow::mChangeState = true;
#endif
	}

	static void Log(MonoString* string)
	{
		std::string cStrName = MonoToString(string);
		CM_CORE_INFO(cStrName);
		//mono_free(cStr);
	}

	static void GetMousePos(Vec2f* outPos)
	{
		Vec2f worldMousePos = Input.GetMouseWorldPosition();
		*outPos = worldMousePos;
	}

	static void SetAlpha(unsigned int entityID, float alpha)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Renderer>())
		{
			go.GetComponent<Renderer>().SetA(alpha);
		}
	}

	static void SetColour(unsigned int entityID, float r, float g, float b)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Renderer>())
		{
			go.GetComponent<Renderer>().SetR(r);
			go.GetComponent<Renderer>().SetG(g);
			go.GetComponent<Renderer>().SetB(b);
		}
	}

	static void ChangeTexture(unsigned int entityID, MonoString* string)
	{
		std::string cStrName = MonoToString(string);
		GameObject& go = gGOFactory->FetchGO(entityID);
		go.GetComponent<Renderer>().Texture(cStrName);
		//mono_free(cStr);
	}

	static void ChangeText(unsigned int entityID, MonoString* string)
	{
		std::string cStrName = MonoToString(string);
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<TextRenderer>())
		{
			go.GetComponent<TextRenderer>().txt = cStrName;
		}
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
		RegisterComponent<Animation>();
		RegisterComponent<StateMachine>();
		RegisterComponent<Renderer>();
		RegisterComponent<TextRenderer>();
	}

	/// <summary>
	/// Register functions to C# side by using the internal mono call
	/// </summary>
	void ScriptFunctions::RegisterFunctions()
	{
		ADD_INTERNAL_CALL(CloseGame);

		// Transform functions
		ADD_INTERNAL_CALL(Transform_GetScale);
		ADD_INTERNAL_CALL(Transform_SetScale);
		ADD_INTERNAL_CALL(Transform_GetLocalPosition);
		ADD_INTERNAL_CALL(Transform_GetPosition);
		ADD_INTERNAL_CALL(Transform_SetPosition);
		ADD_INTERNAL_CALL(Transform_GetDepth);
		ADD_INTERNAL_CALL(Transform_SetDepth);
		ADD_INTERNAL_CALL(Transform_GetTag);

		//Entity functions
		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(Entity_FindEntityWithName);
		ADD_INTERNAL_CALL(Entity_GetParent);
		ADD_INTERNAL_CALL(Entity_FindEntityWithID);
		ADD_INTERNAL_CALL(Destroy);
		ADD_INTERNAL_CALL(CreateNewGameObject);
		ADD_INTERNAL_CALL(GetScriptInstance);
		ADD_INTERNAL_CALL(GetScriptInstanceFromChildren);

		// Rigidbody functions
		ADD_INTERNAL_CALL(RigidBody_ApplyForce);
		ADD_INTERNAL_CALL(RigidBody_ApplyForceWithTime);

		// renderer functions
		ADD_INTERNAL_CALL(SetAlpha);
		ADD_INTERNAL_CALL(SetColour);

		// Anim functions
		ADD_INTERNAL_CALL(Animation_ChangeAnim);
		ADD_INTERNAL_CALL(GetMaxTime);

		// input functions
		ADD_INTERNAL_CALL(IsKeyPressed);
		ADD_INTERNAL_CALL(IsKeyHold);
		ADD_INTERNAL_CALL(IsMousePressed);
		ADD_INTERNAL_CALL(IsMouseReleased);
		ADD_INTERNAL_CALL(GetMousePos);

		// Button function
		ADD_INTERNAL_CALL(ChangeScene);
		ADD_INTERNAL_CALL(ChangeTexture);

		// Sound
		ADD_INTERNAL_CALL(Sound_PlaySFX);
		ADD_INTERNAL_CALL(Sound_PlayBGM);
		ADD_INTERNAL_CALL(Sound_StopBGM);

		// Debug
		ADD_INTERNAL_CALL(Log);

		// FSM
		ADD_INTERNAL_CALL(SetStateCondition);
		ADD_INTERNAL_CALL(GetStateTimer);

		// Text Renderer
		ADD_INTERNAL_CALL(ChangeText);
	}
}