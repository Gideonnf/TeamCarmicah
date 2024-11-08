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
		UInt,
		Vector2,
		Entity
	};

	//struct
	struct ScriptField
	{
		ScriptFieldType mType;
		std::string mName;
		MonoClassField* mClassField;
	};
	
	class ScriptObject
	{
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
		ScriptObject(const std::string& nameSpace, const std::string& className);

		/// <summary>
		/// Instantiates the script object. 
		/// </summary>
		void Instantiate();

		/// <summary>
		/// Set up all the internal function calls by getting the method and the number of params
		/// </summary>
		/// <param name="entity"></param>
		void SetUpEntity(Entity entity);

		/// <summary>
		/// Used to get the method/function out of the C# side script. For example, OnUpdate is the name. Param count is 1 cause float dt
		/// </summary>
		/// <param name="name">Name of the function</param>
		/// <param name="parameterCount">Number of parameters it needs</param>
		/// <returns></returns>
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoMethod* method, void** params = nullptr);

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
	private:
		std::string mNameSpace;
		std::string mClassName;

		// Store what fields the script has
		std::map<std::string, ScriptField> mFields;

		MonoClass* mMonoClass = nullptr;
		MonoObject* mMonoInstance = nullptr;

		MonoMethod* mConstruct = nullptr;
		MonoMethod* mOnCreate = nullptr;
		MonoMethod* mOnUpdate = nullptr;
	};



}

#endif