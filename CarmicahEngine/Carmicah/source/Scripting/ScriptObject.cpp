/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ScriptObject.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:      Script object. Holds the equivalent c# script, references to the standard OnCreate, OnUpdate and OnConstruct functions.
				Used as a shared_ptr in scriptSystem.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


#include <pch.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include "ScriptSystem.h"

#include "ScriptObject.h"
namespace Carmicah
{
#pragma region ScriptClass
	
	ScriptClass::ScriptClass(const std::string& nameSpace, const std::string& className)
	{
		mMonoClass = mono_class_from_name(gScriptSystem->mCoreAssemblyImage, nameSpace.c_str(), className.c_str());
	}

	MonoObject* ScriptClass::Instantiate()
	{
		MonoObject* monoInstance = mono_object_new(gScriptSystem->mAppDomain, mMonoClass);
		mono_runtime_object_init(monoInstance);

		return monoInstance;
		//monoInstance = instance;
		//return instance;
	}

	MonoMethod* ScriptClass::GetMethod(const std::string& name, int varCount)
	{
		return mono_class_get_method_from_name(mMonoClass, name.c_str(), varCount);
	}

	MonoObject* ScriptClass::InvokeMethod(MonoObject* instance, MonoMethod* method, void** params)
	{
		if (!method)
		{
			CM_CORE_ERROR("Instance something something");
			return nullptr;
		}

		if (!instance)
		{
			CM_CORE_ERROR("Instance something something");
			return nullptr;
		}

		if (mono_object_get_class(instance) == nullptr)
		{
			CM_CORE_ERROR("Instance something something");
			return nullptr;
		}

		int paramCount = mono_signature_get_param_count(mono_method_signature(method));
		if (paramCount > 0 && !params)
		{
			CM_CORE_ERROR("Instance something something");
			return nullptr;
		}

		if (!mono_domain_get())
		{
			CM_CORE_ERROR("Instance something something");
			return nullptr;

		}

		
		// Exception so that we can check if any invoke fails
		MonoObject* exception = nullptr;
		MonoObject* result = nullptr;
		try
		{
			result = mono_runtime_invoke(method, instance, params, &exception);
		}
		catch (...)
		{
			CM_CORE_ERROR("Instance something something");
			return nullptr;

		}

		if (exception)
		{
			MonoString* exceptionMsg = mono_object_to_string(exception, nullptr);
			char* errorMsg = mono_string_to_utf8(exceptionMsg);
			std::cerr << "Mono Exception: " << errorMsg << std::endl;
			mono_free(errorMsg);
			return nullptr;
		}

		return result;
	}
#pragma endregion 
	
#pragma region ScriptObject
	
	ScriptObject::ScriptObject(std::shared_ptr<ScriptClass> scClass, Entity entity) : mScriptClass (scClass)
	{
		UNUSED(entity);
		mMonoInstance = scClass->Instantiate();

		// Need to call constructor of entity by getting the entity class thats storing Entity.cs
		mConstruct = gScriptSystem->mEntityClass.GetMethod(".ctor", 1);

		// These are the other functions that every other script that inherits Entity will have
		mOnCreate = scClass->GetMethod("OnCreate", 0);
		mOnUpdate = scClass->GetMethod("OnUpdate", 1);
		mOnFixedUpdate = scClass->GetMethod("OnFixedUpdate", 1);
		mOnClick = scClass->GetMethod("OnClick", 0);
		mOnCollide = scClass->GetMethod("OnCollide", 1);

		mOnMouseEnter = scClass->GetMethod("OnMouseEnter", 0);
		mOnMouseExit = scClass->GetMethod("OnMouseExit", 0);
		mOnMouseHover = scClass->GetMethod("OnMouseHover", 0);

		mOnStateEnter = scClass->GetMethod("OnStateEnter", 1);
		mOnStateUpdate = scClass->GetMethod("OnStateUpdate", 2);
		mOnStateExit = scClass->GetMethod("OnStateExit", 1);

		mHandle = mono_gchandle_new(mMonoInstance, true);
	}

	MonoObject* ScriptObject::GetInstance()
	{
		return mMonoInstance;
	}

	void ScriptObject::InvokeOnConstruct(unsigned int id)
	{
		if (mConstruct)
		{
			// Invoke constructor when setting up entity
			void* param = &id;
			mScriptClass->InvokeMethod(mMonoInstance, mConstruct, &param);

		}
	}

	void ScriptObject::InvokeOnCreate()
	{
		if (mOnCreate)
		{
			mScriptClass->InvokeMethod(mMonoInstance, mOnCreate);
		}
	}

	void ScriptObject::InvokeOnUpdate(float dt)
	{
		if (mOnUpdate)
		{
			void* param = &dt;
			mScriptClass->InvokeMethod(mMonoInstance, mOnUpdate, &param);
		}
	}

	void ScriptObject::InvokeOnFixedUpdate(float dt)
	{
		if (mOnFixedUpdate)
		{
			void* param = &dt;
			mScriptClass->InvokeMethod(mMonoInstance, mOnFixedUpdate, &param);
		}
	}


	void ScriptObject::InvokeOnClick()
	{
		if (mOnClick)
		{

			mScriptClass->InvokeMethod(mMonoInstance, mOnClick);
		}
	}

	void ScriptObject::InvokeOnCollide(unsigned int id)
	{
		//UNUSED(otherID);
		if (mOnCollide)
		{
			void* param = &id;
			mScriptClass->InvokeMethod(mMonoInstance, mOnCollide, &param);
		}
	}


	/// <summary>
	/// Call when a mouse enters the collider box of an object
	/// </summary>
	void ScriptObject::InvokeOnMouseEnter()
	{
		CM_CORE_INFO("Testing");

		if (mOnMouseEnter)
		{
			mScriptClass->InvokeMethod(mMonoInstance, mOnMouseEnter);
		}
	}

	/// <summary>
	/// Call when a mouse leaves the collider box of an object
	/// </summary>
	void ScriptObject::InvokeOnMouseExit()
	{
		CM_CORE_INFO("Testing");
		if (mOnMouseExit)
		{
			mScriptClass->InvokeMethod(mMonoInstance, mOnMouseExit);
		}
	}

	/// <summary>
	/// Call when a mouse is hovering the collider box of an object
	/// </summary>
	void ScriptObject::InvokeOnMouseHover()
	{
		if (mOnMouseHover)
		{
			mScriptClass->InvokeMethod(mMonoInstance, mOnMouseHover);
		}
	}

	void ScriptObject::InvokeOnStateEnter(std::string stateName)
	{
		if (mOnStateEnter)
		{
			MonoString* monoStateName = mono_string_new(mono_domain_get(), stateName.c_str());

			void* param = monoStateName;
			mScriptClass->InvokeMethod(mMonoInstance, mOnStateEnter, &param);

		}
	}

	void ScriptObject::InvokeOnStateUpdate(std::string stateName, float dt)
	{
		if (mOnStateUpdate && mMonoInstance)
		{
			void* param[2];
			MonoString* monoStateName = mono_string_new(mono_domain_get(), stateName.c_str());
			param[0] = monoStateName;
			param[1] = &dt;

			mScriptClass->InvokeMethod(mMonoInstance, mOnStateUpdate, param);
		}
	}

	void ScriptObject::InvokeOnStateExit(std::string stateName)
	{
		if (mOnStateExit)
		{
			MonoString* monoStateName = mono_string_new(mono_domain_get(), stateName.c_str());

			void* param = monoStateName;
			mScriptClass->InvokeMethod(mMonoInstance, mOnStateExit, &param);

		}
	}


	std::shared_ptr<ScriptClass> ScriptObject::GetScriptClass()
	{
		return mScriptClass;
	}

#pragma endregion
}