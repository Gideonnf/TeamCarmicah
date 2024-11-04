#ifndef SCRIPT_OBJECT_H
#define SCRIPT_OBJECT_H
#include "../ECS/ECSTypes.h"

namespace Carmicah
{
	
	class ScriptObject
	{
	public:
		ScriptObject() = default;
		ScriptObject(const std::string& nameSpace, const std::string& className);

		void Instantiate();
		void SetUpEntity(Entity entity);
		MonoMethod* GetMethod(const std::string& name, int parameterCount);
		MonoObject* InvokeMethod(MonoMethod* method, void** params = nullptr);

		void InvokeOnCreate();
		void InvokeOnUpdate(float dt);
	private:
		std::string mNameSpace;
		std::string mClassName;

		MonoClass* mMonoClass = nullptr;
		MonoObject* mMonoInstance = nullptr;

		MonoMethod* mConstruct = nullptr;
		MonoMethod* mOnCreate = nullptr;
		MonoMethod* mOnUpdate = nullptr;
	};



}

#endif