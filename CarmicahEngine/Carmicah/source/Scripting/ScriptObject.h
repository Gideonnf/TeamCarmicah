/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ScriptObject.h

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:      Script object. Holds the equivalent c# script, references to the standard OnCreate, OnUpdate and OnConstruct functions.
				Used as a shared_ptr in scriptSystem. 

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H
#include "../ECS/ECSTypes.h"

namespace Carmicah
{
	// Types that a script field can be
	enum class ScriptFieldType
	{
		None = 0,
		Float,
		Double,
		Bool,
		Char,
		Int,
		Short,
		UInt,
		Vector2,
		Entity,
		String
	};

	//struct
	struct ScriptField
	{
		ScriptFieldType mType;
		std::string mName;
		MonoClassField* mClassField;
	};

	class ScriptClass
	{
	public:
		friend class ScriptSystem;

		std::string mNameSpace;
		std::string mClassName;

		// Store what fields the script has
		std::map<std::string, ScriptField> mFields;

		ScriptClass() = default;
		ScriptClass(const std::string& nameSpace, const std::string& className);


		MonoClass* mMonoClass = nullptr;

		/// <summary>
		/// Instantiates the script object. 
		/// </summary>
		MonoObject* Instantiate();
		/// <summary>
		/// Used to get the method/function out of the C# side script. For example, OnUpdate is the name. Param count is 1 cause float dt
		/// </summary>
		/// <param name="name">Name of the function</param>
		/// <param name="parameterCount">Number of parameters it needs</param>
		/// <returns></returns>
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoObject* instance, MonoMethod* method, void** params = nullptr);


	};
	
	class ScriptObject
	{
	private:
		std::shared_ptr<ScriptClass> mScriptClass;

		MonoObject* mMonoInstance = nullptr;

		MonoMethod* mConstruct = nullptr;
		MonoMethod* mOnCreate = nullptr;
		MonoMethod* mOnUpdate = nullptr;
		MonoMethod* mOnClick = nullptr;
		MonoMethod* mOnCollide = nullptr;

	public:
		friend class ScriptSystem;

		/// <summary>
		/// Default constructor
		/// </summary>
		ScriptObject() = default;

		/// <summary>
		/// Overloaded constructor
		/// </summary>
		/// <param name="nameSpace">Name space of the C# script. Usually always Carmicah</param>
		/// <param name="className">name of the class</param>
		ScriptObject(std::shared_ptr<ScriptClass> scClass, Entity entity);

		/// <summary>
		/// Get the mMonoInstance reference to get script instance for C# side
		/// </summary>
		MonoObject* GetInstance();


		/// <summary>
		/// Set up all the internal function calls by getting the method and the number of params
		/// </summary>
		/// <param name="entity"></param>
		void SetUpEntity(Entity entity);


		/// <summary>
		/// For calling the OnConstruct function 
		/// </summary>
		/// <param name="id"></param>
		void InvokeOnConstruct(unsigned int id);

		/// <summary>
		/// Calls the OnCreate function. This is only called for all entity scripts, when the game is ran in engine
		/// </summary>
		void InvokeOnCreate();

		/// <summary>
		/// Calls the OnUpdate function. This is done every loop when the game is in runtime for every entity
		/// </summary>
		/// <param name="dt">Delta time</param>
		void InvokeOnUpdate(float dt);
		
		/// <summary>
		/// Call when obj is clicked, if it has a script with an onClick function then itll run it
		/// </summary>
		void InvokeOnClick();

		/// <summary>
		/// Call when obj collides, if it has a script with an onCollide function
		/// </summary>
		void InvokeOnCollide(Entity otherID);

		template<typename T>
		T GetFieldValue(const std::string& name)
		{
			const auto& fields = mScriptClass->mFields;
			if (fields.count(name) == 0)
			{
				return T();
			}
			auto iter = fields.find(name);
			const ScriptField& field = iter->second;
			char fieldBuffer[32];
			mono_field_get_value(mMonoInstance, field.mClassField, fieldBuffer);
			//mono_string_to_utf8
			// cast it to the T that is trying to be retrieved and derefence it
			return *(T*)fieldBuffer;
		}

		template<>
		std::string GetFieldValue(const std::string& name)
		{
			const auto& fields = mScriptClass->mFields;
			if (fields.count(name) == 0)
			{
				return std::string();
			}
			auto iter = fields.find(name);
			const ScriptField& field = iter->second;

			std::string result;
			MonoString* monoStr = reinterpret_cast<MonoString*>(mono_field_get_value_object(mono_domain_get(), field.mClassField, mMonoInstance));
			if (monoStr != nullptr)
			{
				char* utf8str = mono_string_to_utf8(monoStr);
				result = utf8str;
				mono_free(utf8str);
			}

			return result;
			//char fieldBuffer[128];
			//mono_field_get_value(mMonoInstance, field.mClassField, fieldBuffer);
			////mono_string_to_utf8
			//// cast it to the T that is trying to be retrieved and derefence it
			//return fieldBuffer;

		}

		template <typename T>
		void SetFieldValue(const std::string& name, T val)
		{
			const auto& fields = mScriptClass->mFields;
			if (fields.count(name) == 0)
			{
				return;
			}
			auto iter = fields.find(name);
			const ScriptField& field = iter->second;

			//void* valPtr =;
			mono_field_set_value(mMonoInstance, field.mClassField, (void*)&val);
		}

	template <>
	void SetFieldValue<std::string>(const std::string& name, std::string val)
	{
		const auto& fields = mScriptClass->mFields;
		if (fields.count(name) == 0)
		{
			return;
		}
		auto iter = fields.find(name);
		const ScriptField& field = iter->second;

		MonoString* monoStr = mono_string_new(mono_domain_get(), val.c_str());
		if (monoStr != nullptr) {
			// Set the MonoString as the value of the field
			mono_field_set_value(mMonoInstance, field.mClassField, monoStr);
		}
	}

		std::shared_ptr<ScriptClass> GetScriptClass();

	};



}

#endif