#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H
#include "../Singleton.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include "ScriptObject.h"

namespace Carmicah
{
	class ScriptSystem : public Singleton<ScriptSystem>
	{
	public:
		ScriptSystem();
		~ScriptSystem();
		void Init();
		void CleanUp();
		void InitMono();

		char* ReadBytes(const std::string& filepath, uint32_t* outSize);
		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		void PrintAssemblyTypes(MonoAssembly* assembly);
		void LogMonoHeapSize();
		MonoDomain* mRootDomain;
		MonoDomain* mAppDomain;
		MonoAssembly* mCoreAssembly;
		MonoImage* mCoreAssemblyImage;

		// Hold a reference to Entity class as it contains the constructor that all entity scripts runs to store mID
		ScriptObject mEntityClass;

		// keep track of every type of entity classes
		std::unordered_map<std::string, ScriptObject> mEntityClasses;
		// keep track of entity to script object
		std::unordered_map<unsigned int, ScriptObject> mEntityInstances;
	};

}

#endif