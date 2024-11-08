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

		ScriptObject() = default;
		ScriptObject(const std::string& nameSpace, const std::string& className);

		void Instantiate();
		void SetUpEntity(Entity entity);
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoMethod* method, void** params = nullptr);

		void InvokeOnConstruct(unsigned int id);
		void InvokeOnCreate();
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