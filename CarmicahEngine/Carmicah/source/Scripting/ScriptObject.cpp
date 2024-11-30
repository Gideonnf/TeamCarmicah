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
		// Exception so that we can check if any invoke fails
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, instance, params, &exception);
	}
#pragma endregion 
	
#pragma region ScriptObject
	
	ScriptObject::ScriptObject(std::shared_ptr<ScriptClass> scClass, Entity entity) : mScriptClass (scClass)
	{
		mMonoInstance = scClass->Instantiate();

		// Need to call constructor of entity by getting the entity class thats storing Entity.cs
		mConstruct = gScriptSystem->mEntityClass.GetMethod(".ctor", 1);

		// These are the other functions that every other script that inherits Entity will have
		mOnCreate = scClass->GetMethod("OnCreate", 0);
		mOnUpdate = scClass->GetMethod("OnUpdate", 1);
		mOnClick = scClass->GetMethod("OnClick", 0);
		mOnCollide = scClass->GetMethod("OnCollide", 1);
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
			mScriptClass->InvokeMethod(mMonoInstance, mOnCreate);
	}

	void ScriptObject::InvokeOnUpdate(float dt)
	{
		if (mOnUpdate)
		{
			void* param = &dt;
			mScriptClass->InvokeMethod(mMonoInstance, mOnUpdate, &param);
		}
	}

	void ScriptObject::InvokeOnClick()
	{
		if (mOnClick)
			mScriptClass->InvokeMethod(mMonoInstance, mOnClick);
	}

	void ScriptObject::InvokeOnCollide(Entity otherID)
	{
		if (mOnCollide)
		{
			mScriptClass->InvokeMethod(mMonoInstance, mOnCollide);
		}
	}

	std::shared_ptr<ScriptClass> ScriptObject::GetScriptClass()
	{
		return mScriptClass;
	}

#pragma endregion
}