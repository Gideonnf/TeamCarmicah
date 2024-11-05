#include <pch.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include "ScriptSystem.h"

#include "ScriptObject.h"
namespace Carmicah
{
	ScriptObject::ScriptObject(const std::string& nameSpace, const std::string& className)
	{
		mMonoClass = mono_class_from_name(gScriptSystem->mCoreAssemblyImage, nameSpace.c_str(), className.c_str());
	}

	void ScriptObject::Instantiate()
	{
		mMonoInstance = mono_object_new(gScriptSystem->mAppDomain, mMonoClass);
		mono_runtime_object_init(mMonoInstance);

		//monoInstance = instance;
		//return instance;
	}

	void ScriptObject::SetUpEntity(Entity entity)
	{
		Instantiate();
		// Need to call constructor of entity by getting the entity class thats storing Entity.cs
		mConstruct = gScriptSystem->mEntityClass.GetMethod(".ctor", 1);

		// These are the other functions that every other script that inherits Entity will have
		mOnCreate = GetMethod("OnCreate", 0);
		mOnUpdate = GetMethod("OnUpdate", 1);


	}

	void ScriptObject::InvokeOnCreate()
	{
		if (mOnCreate)
			InvokeMethod(mOnCreate);
	}

	void ScriptObject::InvokeOnUpdate(float dt)
	{
		if (mOnUpdate)
		{
			void* param = &dt;
			InvokeMethod(mOnUpdate, &param);
		}
	}

	MonoMethod* ScriptObject::GetMethod(const std::string& name, int varCount)
	{
		return mono_class_get_method_from_name(mMonoClass, name.c_str(), varCount);
	}

	MonoObject* ScriptObject::InvokeMethod(MonoMethod* method, void** params)
	{
		// Exception so that we can check if any invoke fails
		MonoObject* exception = nullptr;
		return mono_runtime_invoke(method, mMonoInstance, params, &exception);
	}
}