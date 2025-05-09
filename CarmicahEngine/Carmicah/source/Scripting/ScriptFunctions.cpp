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
#include "../Components/Collider2D.h"
#include "../Components/Animation.h"
#include "ScriptSystem.h"
#include "../Input/InputSystem.h"
#include "../Systems/SoundSystem.h"
#include "../ECS/SystemManager.h"
#include "../Systems/CollisionSystem.h"
#include "../Physics/PhysicsSystem.h"
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

		if (go.HasComponent<Transform>())
			*outScale = go.GetComponent<Transform>().Scale();
		else if (go.HasComponent<UITransform>())
			*outScale = go.GetComponent<UITransform>().Scale();
	}

	/// <summary>
	/// Set the transform's scale
	/// </summary>
	/// <param name="entityID">ID of entity</param>
	/// <param name="inScale"> Take in a vec2f to set the scale with</param>
	static void Transform_SetScale(unsigned int entityID, Vec2f* inScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		if (go.HasComponent<Transform>())
			go.GetComponent<Transform>().Scale(*inScale);
		else if (go.HasComponent<UITransform>())
			go.GetComponent<UITransform>().Scale(*inScale);
	}

	static void Transform_GetRenderingScale(unsigned int entityID, Vec2f* outScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (!go.HasComponent<Renderer>())
			return;
		const Texture& t = AssetManager::GetInstance()->GetAsset<Texture>(ComponentManager::GetInstance()->GetComponent<Renderer>(entityID).Texture());

		if (go.HasComponent<Transform>())
		{
			*outScale = Vec2f((AssetManager::GetInstance()->enConfig.maxTexSize / 50.f * t.mtx.m00), (AssetManager::GetInstance()->enConfig.maxTexSize / 50.f * t.mtx.m11));
		}
		else if (go.HasComponent<UITransform>())
		{
			*outScale = Vec2f((AssetManager::GetInstance()->enConfig.maxTexSize * t.mtx.m00), (AssetManager::GetInstance()->enConfig.maxTexSize * t.mtx.m11));
		}

	}

	static void Transform_GetWorldScale(unsigned int entityID, Vec2f* outScale)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
		{
			*outScale = go.GetComponent<Transform>().Scale() * go.GetComponent<Transform>().accumulatedScale;
		}
		else if (go.HasComponent<UITransform>())
		{
			*outScale = go.GetComponent<UITransform>().Scale() * go.GetComponent<UITransform>().accumulatedScale;
		}

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

	static MonoArray* Entity_FindEntitiesWithTag(MonoString* tag)
	{
		std::string cStrName = MonoToString(tag);

		std::vector<Entity> entityIDs = gGOFactory->GetEntitiesWithTag(cStrName);


		MonoDomain* domain = mono_domain_get();
		MonoArray* monoArray = mono_array_new(domain, mono_get_uint32_class(), entityIDs.size());

		for (size_t i = 0; i < entityIDs.size(); ++i)
		{
			mono_array_set(monoArray, uint32_t, i, entityIDs[i]);
		}

		return monoArray;
	}

	static MonoArray* Entity_GetAllChildren(unsigned int entityID)
	{
		std::vector<Entity> entityIDs;
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
		{
			Transform& goTransform = go.GetComponent<Transform>();
			if (goTransform.children.size() > 0)
			{
				for each (Entity entity in goTransform.children)
				{
					entityIDs.push_back(entity);
				}
			}
		}
		else if (go.HasComponent<UITransform>())
		{
			UITransform& goTransform = go.GetComponent<UITransform>();
			if (goTransform.children.size() > 0)
			{
				for each (Entity entity in goTransform.children)
				{
					entityIDs.push_back(entity);
				}
			}
		}

		MonoDomain* domain = mono_domain_get();
		MonoArray* monoArray = mono_array_new(domain, mono_get_uint32_class(), entityIDs.size());

		for (size_t i = 0; i < entityIDs.size(); ++i)
		{
			mono_array_set(monoArray, uint32_t, i, entityIDs[i]);
		}

		return monoArray;
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
			return go.GetComponent<Transform>().ParentID();
		}
		else if (go.HasComponent<UITransform>())
		{
			return go.GetComponent<UITransform>().ParentID();
		}

		return 0;
	}

	/// <summary>
	/// Returns the first child 
	/// </summary>
	/// <param name="entityID"></param>
	/// <returns></returns>
	static unsigned int Entity_GetChild(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
		{
			if (go.GetComponent<Transform>().children.size() > 0)
				return go.GetComponent<Transform>().children[0];
		}
		else if (go.HasComponent<UITransform>())
		{
			if (go.GetComponent<UITransform>().children.size() > 0)
				return go.GetComponent<UITransform>().children[0];
		}

		return 0;
	}

	/// <summary>SS
	/// Internal function call to play sound effects between C# and C++
	/// </summary>
	/// <param name="name">Name of the sound file to play</param>
	static void Sound_PlaySFX(MonoString* name, float volume, bool isLoop, unsigned int entityID)
	{
		std::string cStrName = MonoToString(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySoundThis(cStrName, SoundCategory::SFX, SoundSystem::SOUND_INGAME, isLoop, volume, entityID);
		//mono_free(cStrname);
	}

	/**
	 * @brief Plays background music (BGM) with a specified volume.
	 *
	 * This function converts the provided string for the BGM file name into a C-style string, retrieves the SoundSystem from the
	 * SystemManager, and plays the background music (BGM) with the given volume.
	 *
	 * @param name The name of the BGM file to be played.
	 * @param volume The volume at which the BGM will be played, where 1.0 is the maximum volume.
	 *
	 * @return void
	 */
	static void Sound_PlayBGM(MonoString* name, float volume)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PlaySoundThis(cStrname, SoundCategory::BGM, SoundSystem::SOUND_BGM, true, volume , 0);
		mono_free(cStrname);
	}

	/**
	 * @brief Stops the currently playing background music (BGM).
	 *
	 * This function converts the provided string for the BGM file name into a C-style string, retrieves the SoundSystem from the
	 * SystemManager, and stops the currently playing BGM.
	 *
	 * @param name The name of the BGM file to be stopped.
	 *
	 * @return void
	 */
	static void Sound_StopBGM(MonoString* name)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSound(SoundSystem::SOUND_BGM);
		mono_free(cStrname);
	}

	/**
	 * @brief Stops the currently playing sound effect (SFX).
	 *
	 * This function converts the provided string for the SFX file name into a C-style string, retrieves the SoundSystem from the
	 * SystemManager, and stops the currently playing SFX.
	 *
	 * @param name The name of the SFX file to be stopped.
	 *
	 * @return void
	 */
	static void Sound_StopSFX(MonoString* name, unsigned int entityID)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSoundSFX(SoundSystem::SOUND_INGAME, entityID);
		mono_free(cStrname);
	}

	/**
	 * @brief Stops all currently playing sound effects (SFX).
	 *
	 * This function retrieves the SoundSystem from the SystemManager and stops all currently playing SFX.
	 *
	 * @return void
	 */
	static void Sound_StopAllSFX()
	{
		//char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSound(SoundSystem::SOUND_INGAME);
		//mono_free(cStrname);
	}

	/**
	 * @brief Stops the currently playing sound effect (SFX) with a fade out effect.
	 *
	 * This function converts the provided string for the SFX file name into a C-style string, retrieves the SoundSystem from the
	 * SystemManager, and stops the currently playing SFX with a fade out effect over the given duration.
	 *
	 * @param name The name of the SFX file to be stopped.
	 * @param fadeTimer The current time passed before the fade effect starts.
	 * @param fadeDuration The duration over which the fade effect will occur.
	 *
	 * @return void
	 */
	static void Sound_StopSFXWithFade(MonoString* name, float fadeTimer, float fadeDuration)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSoundWithFade(SoundSystem::SOUND_INGAME, fadeTimer, fadeDuration);
		mono_free(cStrname);
	}

	/**
	 * @brief Stops the currently playing background music (BGM) with a fade out effect.
	 *
	 * This function converts the provided string for the BGM file name into a C-style string, retrieves the SoundSystem from the
	 * SystemManager, and stops the currently playing BGM with a fade out effect over the given duration.
	 *
	 * @param name The name of the BGM file to be stopped.
	 * @param fadeTimer The current time passed before the fade effect starts.
	 * @param fadeDuration The duration over which the fade effect will occur.
	 *
	 * @return void
	 */
	static void Sound_StopBGMWithFade(MonoString* name, float fadeTimer, float fadeDuration)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->StopSoundWithFade(SoundSystem::SOUND_BGM, fadeTimer, fadeDuration);
		mono_free(cStrname);
	}

	/**
	 * @brief Switches the background music (BGM) to a new track with optional fade effects.
	 *
	 * This function converts the provided string for the new BGM file name into a C-style string, retrieves the SoundSystem from
	 * the SystemManager, and switches to the new BGM track with optional fade effects, looping, and volume settings.
	 *
	 * @param name The name of the new BGM file to be played.
	 * @param fadeTimer The current time passed before the fade effect starts.
	 * @param fadeDuration The duration over which the fade effect will occur.
	 * @param isLoop Whether the new BGM should loop.
	 * @param fadeInNext Whether to fade in the new BGM.
	 *
	 * @return void
	 */
	static void Sound_SwitchBGM(MonoString* name, float fadeTimer, float fadeDuration, bool isLoop, bool fadeInNext)
	{
		char* cStrname = mono_string_to_utf8(name);
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->SwitchSound(SoundSystem::SOUND_BGM, cStrname, SoundCategory::BGM, isLoop, 0.4f, fadeTimer, fadeDuration, fadeInNext);
		mono_free(cStrname);
	}
	
	/**
	 * @brief Toggles the muffle effect on sound effects (SFX) for a specific entity.
	 *
	 * This function retrieves the SoundSystem from the SystemManager and toggles the muffle effect on the SFX of a specific entity
	 * by adjusting the volume and applying a low-pass filter effect.
	 *
	 * @param toMuffle Whether to apply or remove the muffle effect.
	 * @param entityID The ID of the entity whose SFX will be affected.
	 *
	 * @return void
	 */
	static void Sound_ToggleMuffleSFX(bool toMuffle, unsigned int entityID)
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->ToggleMuffle(SoundSystem::SOUND_INGAME, toMuffle, entityID);
	}

	/**
	 * @brief Toggles the muffle effect on background music (BGM).
	 *
	 * This function retrieves the SoundSystem from the SystemManager and toggles the muffle effect on the currently playing BGM
	 * by adjusting the volume and applying a low-pass filter effect.
	 *
	 * @param toMuffle Whether to apply or remove the muffle effect.
	 *
	 * @return void
	 */
	static void Sound_ToggleMuffleBGM(bool toMuffle)
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->ToggleMuffle(SoundSystem::SOUND_BGM, toMuffle, 0);
	}

	static void Sound_PauseAllSounds()
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->PauseAllSounds();
	}

	static void Sound_ResumeAllSounds()
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		souSystem->ResumeAllSounds();
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

	static void Sound_SetCategoryVolume(int category, float volume)
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		if (category == 0)
		{
			souSystem->SetCategoryVolume(SoundCategory::SFX, SoundSystem::SOUND_INGAME, volume);
			souSystem->SetCategoryVolume(SoundCategory::UI, SoundSystem::SOUND_INMENU, volume);
		}
		else if (category == 1)
		{
			//souSystem->SetCategoryVolume(SoundCategory::EDITOR, SoundSystem::SOUND_BGM, volume);
			souSystem->SetCategoryVolume(SoundCategory::BGM, SoundSystem::SOUND_BGM, volume);
		}
	}

	static void Sound_GetCategoryVolume(int category)
	{
		auto souSystem = SystemManager::GetInstance()->GetSystem<SoundSystem>();
		if (category == 0)
		{
			souSystem->GetCategoryVolume(SoundCategory::SFX);
		}
		else if (category == 1)
		{
			souSystem->GetCategoryVolume(SoundCategory::BGM);
		}
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

	static void RigidBody_Move(unsigned int entityID, Vec2f pos)
	{
		// ideally this should be in rigidbody system or smth but idk idc now

		GameObject& go = gGOFactory->FetchGO(entityID);
		auto colSys = SystemManager::GetInstance()->GetSystem<CollisionSystem>();

		// move it to it's new pos
		//Vec2f oldPos = go.GetComponent<Transform>().Pos();
		////CM_CORE_INFO("Old pos {},{}", oldPos.x, oldPos.y);
		//go.GetComponent<Transform>().Pos(pos);
		//CM_CORE_INFO("Old pos {},{}", go.GetComponent<Transform>().Pos().x, go.GetComponent<Transform>().Pos().y);

		// if its not colliding with something then set the position
		if (!colSys->CollisionCheck(entityID, pos))
		{
			go.GetComponent<Transform>().Pos(go.GetComponent<Transform>().Pos() + pos);
		}
	}

	static void RigidBody_StopForces(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		if (go.HasComponent<RigidBody>())
		{
			go.GetComponent<RigidBody>().forcesManager.RemoveForce();
		}
	}

	/// <summary>
	/// Internal call for stopping the object
	/// </summary>
	static void RigidBody_StopObject(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		

		if (go.HasComponent<RigidBody>())
		{
			auto phySystem = SystemManager::GetInstance()->GetSystem<PhysicsSystem>();
			phySystem->StopObject(entityID);
		}
	}

	static void Transform_GetLocalPosition(unsigned int entityID, Vec2f* outPos)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);

		if (go.HasComponent<Transform>())
		{
			*outPos = go.GetComponent<Transform>().Pos();
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
			if (go.GetComponent<Transform>().ParentID() != 0)
			{
				//Transform parentTransform = ComponentManager::GetInstance()->GetComponent<Transform>(go.GetComponent<Transform>().parent);
				//*outPos += parentTransform.Pos();
				
			}
			*outPos = go.GetComponent<Transform>().ExtractWorldPos();
			//CM_CORE_INFO("outPos {}, {}", outPos->x, outPos->y);
			//CM_CORE_INFO("worldPos {}, {}", go.GetComponent<Transform>().ExtractWorldPos().x, go.GetComponent<Transform>().ExtractWorldPos().y);
		}
		else if (go.HasComponent<UITransform>())
		{
			
			//*outPos = go.GetComponent<UITransform>().Pos();
			*outPos = go.GetComponent<UITransform>().ExtractWorldPos();

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

		auto transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();
		transformSys->UpdateTransform(entityID);
	}

	/// <summary>
	/// Internal call for getting the FPS Counter
	/// </summary>
	static double Time_GetFPS()
	{
		return CarmicahTime::GetInstance()->FPS();
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
		//if (Input.IsMouseReleased(De))
		//{
		//	CM_CORE_INFO("LEFT MOUSE BUTTON RELEASE");
		//}
		return Input.IsMouseReleased(button);
	}

	static float GetMouseScroll()
	{
		return Input.GetScrollOffset();
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


		auto transformSys = SystemManager::GetInstance()->GetSystem<TransformSystem>();
		transformSys->UpdateTransform(newGO.GetID());
		//mono_free(cStrName);

		return newGO.GetID();
	}

	static MonoObject* GetScriptInstance(unsigned int entityID, MonoString* baseName)
	{
		if (gScriptSystem->mEntityInstances.count(entityID) == 0)
		{
			CM_CORE_ERROR("Entity does not have script attached");
			return nullptr;
		}

		std::string cStrName = MonoToString(baseName);

		if (gScriptSystem->mEntityInstances.count(entityID) > 0)
		{
			return gScriptSystem->mEntityInstances[entityID]->GetInstance();
		}


		return nullptr;
	}

	static bool HasScriptInstance(unsigned int entityID, MonoString* baseName)
	{
		if (gScriptSystem->mEntityInstances.count(entityID) == 0)
		{
			//CM_CORE_ERROR("Entity does not have script attached");
			return false;
		}

		std::string cStrName = MonoToString(baseName);
		if (gScriptSystem->mEntityInstances.count(entityID) > 0)
		{
			if (gScriptSystem->mEntityInstances[entityID]->GetScriptClass()->mClassName == cStrName)
				return true;
		}

		return false;
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

		return nullptr;

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
		if (go.HasComponent<Transform>())
			*outFloat = go.GetComponent<Transform>().GetDepth();
		else if(go.HasComponent<UITransform>())
			*outFloat = go.GetComponent<UITransform>().GetDepth();
		else
			*outFloat = 0.0f;
	}

	static void Transform_SetDepth(unsigned int entityID, float* inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
			go.GetComponent<Transform>().Depth(*inFloat);
		else if (go.HasComponent<UITransform>())
			go.GetComponent<UITransform>().Depth(*inFloat);
	}

	static void Transform_GetRot(unsigned int entityID, float* outFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
			*outFloat = go.GetComponent<Transform>().GetRot();
		else if (go.HasComponent<UITransform>())
			*outFloat = go.GetComponent<UITransform>().GetRot();
		else
			*outFloat = 0.0f;
	}

	static void Transform_SetRot(unsigned int entityID, float* inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Transform>())
			go.GetComponent<Transform>().Rot(*inFloat);
		else if (go.HasComponent<UITransform>())
			go.GetComponent<UITransform>().Rot(*inFloat);
	}

	static void GetRedColour(unsigned int entityID, float* outFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Renderer>())
		{
			*outFloat = go.GetComponent<Renderer>().GetR();
		}
		else
		{
			*outFloat = 0.0f;
		}
	}
	
	static void SetRedColour(unsigned int entityID, float* inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Renderer>())
		{
			go.GetComponent<Renderer>().SetR(*inFloat);
		}
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

		std::string ret = "";
		return mono_string_new(mono_domain_get(), ret.c_str());
	}

	static MonoString* GetFilePath()
	{
		/*std::filesystem::path fileDir = AssetManager::GetInstance()->enConfig.assetLoc;*/
		std::string filePath = (std::filesystem::current_path().parent_path() / "Assets").string();
		CM_CORE_INFO("File Path {}", filePath);
		return mono_string_new(mono_domain_get(), filePath.c_str());
	}

	static void Animation_ChangeAnim(unsigned int entityID, MonoString* string)
	{
		std::string cStrName = MonoToString(string);
		GameObject& go = gGOFactory->FetchGO(entityID);
		//CM_CORE_INFO("Entity ID in changeAnim: {}", entityID);
		//go.GetComponent<Animation>().animAtlas = cStr;
		if (go.HasComponent<Animation>())
			go.GetComponent<Animation>().ChangeAnim(cStrName);
		//mono_free(cStr);
	}

	static void Animation_Pause(unsigned int entityID, bool val)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Animation>())
		{
			go.GetComponent<Animation>().paused = val;
		}
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
		return 0.0f;
	}

	static float Animation_GetCurrFrameTime(unsigned entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Animation>())
		{
			Animation& a{ go.GetComponent<Animation>() };
			return a.maxTime;
		}

		return 0.0f;
	}
	
	static int Animation_GetCurrFrameNo(unsigned entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Animation>())
		{
			Animation& a{ go.GetComponent<Animation>() };
			return a.currPiece;
		}

		return 0;

	}

	static bool Animation_IsAnimFinished(unsigned entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Animation>())
		{
			Animation& a{ go.GetComponent<Animation>() };
			return a.animState == Animation::ANIM_CODE::STOP_ANIM;
		}

		return 0;
	}

	//static void SetCollisionLayer(unsigned int entityID, unsigned int layer)
	//{
	//	GameObject& go = gGOFactory->FetchGO(entityID);
	//	unsigned int& prevColMask = go.GetComponent<Transform>().collisionMask;
	//	uint32_t prevLayerBit = 1 << prevColMask;

	//	//go.AddCollisionLayer(CollisionLayer(layer));
	//	uint32_t newLayer = 1 << layer;


	//	go.AddCollisionLayer(static_cast<CollisionLayer>(newLayer));
	//	go.RemoveCollisionLayer(static_cast<CollisionLayer>(prevLayerBit));
	//}

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
	
		auto fsmSystem = SystemManager::GetInstance()->GetSystem<FSMSystem>();
	
		fsmSystem->SetCondition(entityID, var);
	}

	static float GetStateTimer(unsigned int entityID)
	{
		auto fsmSystem = SystemManager::GetInstance()->GetSystem<FSMSystem>();

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

	static MonoString* GetSceneName()
	{
		auto sceneSystem = SystemManager::GetInstance()->GetSystem<SceneSystem>();
		return mono_string_new(mono_domain_get(), sceneSystem->GetCurrScene().c_str());
	}

	static void GetMousePos(Vec2f* outPos)
	{
		Vec2f worldMousePos = Input.GetMouseWorldPosition();
		*outPos = worldMousePos;
	}

	static void GetMouseUIPos(Vec2f* outPos)
	{
		Vec2f UIMousePos = Input.GetMousePosition();
		*outPos = UIMousePos;
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

	static float TextGetWidth(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<TextRenderer>())
		{
			return go.GetComponent<TextRenderer>().totalWidth;
		}
		return 0;
	}

	static bool GetParticlesActive(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<ParticleEmitter>())
		{
			return go.GetComponent<ParticleEmitter>().HasEmitterQualities(ParticleEmitter::EMITTER_ACTIVE);
		}
		return false;
	}

	static void SetParticlesActive(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<ParticleEmitter>())
		{
			return go.GetComponent<ParticleEmitter>().SetEmitterQualities(ParticleEmitter::EMITTER_ACTIVE, true);
		}
	}

	static void Particles_SetColor(unsigned int entityID, float r, float g, float b)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<ParticleEmitter>())
		{
			go.GetComponent<ParticleEmitter>().colR = r;
			go.GetComponent<ParticleEmitter>().colG = g;
			go.GetComponent<ParticleEmitter>().colB = b;
		}
	}


	static float Collider2D_GetCustomWidth(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			return go.GetComponent<Collider2D>().GetCustomWidth();
		return 0;
	}

	static float Collider2D_GetCustomHeight(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			return go.GetComponent<Collider2D>().GetCustomHeight();
		return 0;
	}

	static void Collider2D_SetCustomWidth(unsigned int entityID, float inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			go.GetComponent<Collider2D>().CustomWidth(inFloat);
	}

	static void Collider2D_SetCustomHeight(unsigned int entityID, float inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			go.GetComponent<Collider2D>().CustomHeight(inFloat);
	}

	static float Collider2D_GetxPivot(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			return go.GetComponent<Collider2D>().GetxPivot();
		return 0;
	}

	static float Collider2D_GetyPivot(unsigned int entityID)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			return go.GetComponent<Collider2D>().GetyPivot();
		return 0;
	}

	static void Collider2D_SetxPivot(unsigned int entityID, float inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			go.GetComponent<Collider2D>().SetxPivot(inFloat);
	}

	static void Collider2D_SetyPivot(unsigned int entityID, float inFloat)
	{
		GameObject& go = gGOFactory->FetchGO(entityID);
		if (go.HasComponent<Collider2D>())
			go.GetComponent<Collider2D>().SetyPivot(inFloat);
	}

	static bool IsDoneLoading()
	{
#ifdef CM_INSTALLER

		return AssetManager::GetInstance()->doneLoading;
#endif
		return true;
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
		RegisterComponent<Collider2D>();
		RegisterComponent<RigidBody>();
		RegisterComponent<Animation>();
		RegisterComponent<StateMachine>();
		RegisterComponent<Renderer>();
		RegisterComponent<TextRenderer>();
		RegisterComponent<ParticleEmitter>();
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
		ADD_INTERNAL_CALL(Transform_GetRenderingScale);
		ADD_INTERNAL_CALL(Transform_GetWorldScale);
		ADD_INTERNAL_CALL(Transform_GetLocalPosition);
		ADD_INTERNAL_CALL(Transform_GetPosition);
		ADD_INTERNAL_CALL(Transform_SetPosition);
		ADD_INTERNAL_CALL(Transform_GetDepth);
		ADD_INTERNAL_CALL(Transform_SetDepth);
		ADD_INTERNAL_CALL(Transform_GetTag);
		ADD_INTERNAL_CALL(Transform_GetRot);
		ADD_INTERNAL_CALL(Transform_SetRot);

		//Entity functions
		ADD_INTERNAL_CALL(Entity_HasComponent);
		ADD_INTERNAL_CALL(Entity_FindEntityWithName);
		ADD_INTERNAL_CALL(Entity_FindEntitiesWithTag);
		ADD_INTERNAL_CALL(Entity_GetAllChildren);
		ADD_INTERNAL_CALL(Entity_GetChild);
		ADD_INTERNAL_CALL(Entity_GetParent);
		ADD_INTERNAL_CALL(Entity_FindEntityWithID);
		ADD_INTERNAL_CALL(Destroy);
		ADD_INTERNAL_CALL(CreateNewGameObject);
		ADD_INTERNAL_CALL(GetScriptInstance);
		ADD_INTERNAL_CALL(GetScriptInstanceFromChildren);
		ADD_INTERNAL_CALL(HasScriptInstance);
		//ADD_INTERNAL_CALL(SetCollisionLayer);

		//Time functions
		ADD_INTERNAL_CALL(Time_GetFPS);

		// Rigidbody functions
		ADD_INTERNAL_CALL(RigidBody_ApplyForce);
		ADD_INTERNAL_CALL(RigidBody_ApplyForceWithTime);
		ADD_INTERNAL_CALL(RigidBody_StopForces);
		ADD_INTERNAL_CALL(RigidBody_StopObject);
		ADD_INTERNAL_CALL(RigidBody_Move);

		// renderer functions
		ADD_INTERNAL_CALL(SetAlpha);
		ADD_INTERNAL_CALL(SetColour);
		ADD_INTERNAL_CALL(GetRedColour);
		ADD_INTERNAL_CALL(SetRedColour);

		// Anim functions
		ADD_INTERNAL_CALL(Animation_ChangeAnim);
		ADD_INTERNAL_CALL(GetMaxTime);
		ADD_INTERNAL_CALL(Animation_GetCurrFrameTime);
		ADD_INTERNAL_CALL(Animation_GetCurrFrameNo);
		ADD_INTERNAL_CALL(Animation_IsAnimFinished);
		ADD_INTERNAL_CALL(Animation_Pause);

		// input functions
		ADD_INTERNAL_CALL(IsKeyPressed);
		ADD_INTERNAL_CALL(IsKeyHold);
		ADD_INTERNAL_CALL(IsMousePressed);
		ADD_INTERNAL_CALL(IsMouseReleased);
		ADD_INTERNAL_CALL(GetMousePos);
		ADD_INTERNAL_CALL(GetMouseUIPos);
		ADD_INTERNAL_CALL(GetMouseScroll);

		// Button function
		ADD_INTERNAL_CALL(ChangeScene);
		ADD_INTERNAL_CALL(ChangeTexture);

		// Sound
		ADD_INTERNAL_CALL(Sound_PlaySFX);
		ADD_INTERNAL_CALL(Sound_PlayBGM);
		ADD_INTERNAL_CALL(Sound_StopBGM);
		ADD_INTERNAL_CALL(Sound_StopSFX);
		ADD_INTERNAL_CALL(Sound_StopAllSFX);
		ADD_INTERNAL_CALL(Sound_StopSFXWithFade);
		ADD_INTERNAL_CALL(Sound_StopBGMWithFade);
		ADD_INTERNAL_CALL(Sound_SwitchBGM);
		ADD_INTERNAL_CALL(Sound_ToggleMuffleSFX);
		ADD_INTERNAL_CALL(Sound_ToggleMuffleBGM);
		ADD_INTERNAL_CALL(Sound_SetCategoryVolume);
		ADD_INTERNAL_CALL(Sound_GetCategoryVolume);
		ADD_INTERNAL_CALL(Sound_PauseAllSounds);
		ADD_INTERNAL_CALL(Sound_ResumeAllSounds);

		// Debug
		ADD_INTERNAL_CALL(Log);

		// SceneManager
		ADD_INTERNAL_CALL(GetSceneName);

		// FSM
		ADD_INTERNAL_CALL(SetStateCondition);
		ADD_INTERNAL_CALL(GetStateTimer);

		// Text Renderer
		ADD_INTERNAL_CALL(ChangeText);
		ADD_INTERNAL_CALL(TextGetWidth);

		ADD_INTERNAL_CALL(GetFilePath);

		// Particles
		ADD_INTERNAL_CALL(GetParticlesActive);
		ADD_INTERNAL_CALL(SetParticlesActive);
		ADD_INTERNAL_CALL(Particles_SetColor);

		//Collider2D
		ADD_INTERNAL_CALL(Collider2D_GetCustomHeight);
		ADD_INTERNAL_CALL(Collider2D_GetCustomWidth);
		ADD_INTERNAL_CALL(Collider2D_SetCustomHeight);
		ADD_INTERNAL_CALL(Collider2D_SetCustomWidth);
		ADD_INTERNAL_CALL(Collider2D_GetxPivot);
		ADD_INTERNAL_CALL(Collider2D_GetyPivot);
		ADD_INTERNAL_CALL(Collider2D_SetxPivot);
		ADD_INTERNAL_CALL(Collider2D_SetyPivot);

		ADD_INTERNAL_CALL(IsDoneLoading);
	}
}