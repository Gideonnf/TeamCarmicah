#ifndef SCRIPT_SYSTEM_H
#define SCRIPT_SYSTEM_H
#include "../Singleton.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include "ECS/BaseSystem.h"
#include "ScriptObject.h"

namespace Carmicah
{
	class ScriptSystem : public BaseSystem
	{
	public:
		ScriptSystem();
		~ScriptSystem();
		// System Functions
		void EntityDestroyed(Entity) override;
		
		// Initialising and Clean up
		void Init();
		void CleanUp();
		void InitMono();


		char* ReadBytes(const std::string& filepath, uint32_t* outSize);
		MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath);
		void LoadMonoAssembly(const std::string& assemblyPath);
		void PrintAssemblyTypes(MonoAssembly* assembly);
		void LoadEntityClasses();
		void LogMonoHeapSize();
		bool HasEntityClass(std::string scriptName);

		/*!
		OnStart() -> Called when play button is pressed. Loop through all entities and get a reference to their scripts
		OnUpdate() -> Calls the script's update
		OnEnd() -> Clears the entity instance map for next play
		*/
		void OnStart(); // Calls the Enter function of all game objects
		void OnUpdate(float dt);
		void OnEnd();
		// Variables
		MonoDomain* mRootDomain;
		MonoDomain* mAppDomain;
		MonoAssembly* mCoreAssembly;
		MonoImage* mCoreAssemblyImage;

		// Hold a reference to Entity class as it contains the constructor that all entity scripts runs to store mID
		ScriptObject mEntityClass;

		// keep track of every type of entity classes
		std::unordered_map<std::string, std::shared_ptr<ScriptObject>> mEntityClasses;
		// keep track of entity to script object
		std::unordered_map<unsigned int, std::shared_ptr<ScriptObject>> mEntityInstances;
	};

	extern ScriptSystem* gScriptSystem;
}

#endif