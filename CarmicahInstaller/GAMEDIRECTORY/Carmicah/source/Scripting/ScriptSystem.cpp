/*-------------------------------------------------------------------------------------------------------------------------------------------------------------------------
 file:        ScriptSystem.cpp

 author:   Gideon Francis

 email:       g.francis@digipen.edu

 brief:      Script System. cpp file containing the function definitions for the script System.
                Initialises C# mono and cleans it up when its done. Load the mono assembly for the C# Project. Holds a map
                of all the entity classes and the instances of entities

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written consent of
DigiPen Institute of Technology is prohibited.
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------*/


#include "pch.h"
#include "ScriptSystem.h"
#include "../log.h"
#include <filesystem>
#include <mono/metadata/mono-gc.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/tabledefs.h>
#include "../Systems/SceneSystem.h"
#include "../ECS/SystemManager.h"
#include "ScriptFunctions.h"
namespace Carmicah
{
    ScriptSystem* gScriptSystem = NULL;

    static std::unordered_map<std::string, ScriptFieldType> sFieldTypeMap =
    {
        {"System.Single", ScriptFieldType::Float},
        {"System.Double", ScriptFieldType::Double},
        {"System.Boolean", ScriptFieldType::Bool},
        {"System.Char", ScriptFieldType::Char},
        {"System.Int16", ScriptFieldType::Short},
        {"System.Int32", ScriptFieldType::Int},
        {"System.UInt32", ScriptFieldType::UInt},
        {"System.String", ScriptFieldType::String},
        {"Carmicah.Vector2", ScriptFieldType::Vector2},
        {"Carmicah.Entity", ScriptFieldType::Entity},
    };

    ScriptSystem::ScriptSystem()
    {
        mSignature.set(ComponentManager::GetInstance()->GetComponentID<Script>());

        SystemManager::GetInstance()->SetSignature<ScriptSystem>(mSignature);

        if (gScriptSystem == NULL)
            gScriptSystem = this;

         mRootDomain = nullptr;
        mAppDomain = nullptr;
        mCoreAssembly = nullptr;
        mCoreAssemblyImage = nullptr;
        //mEntityClass = nullptr;
    }

    ScriptSystem::~ScriptSystem()
    {
       // CleanUp();
    }

    void ScriptSystem::EntityDestroyed(Entity id)
    {
        // If an entity is destroyed, remove it from mEntityInstances
        // can also take this part to call ondestroy if we do that
        // mEntityInstances is usually cleared at the end of playing
        // but if entity is destroyed in run time then we have to clear it from the map
        for (auto& it : mEntityInstances)
        {
            if (it.first == id)
            {
                // can call ondestroy here maybe if we do that
                //CM_CORE_INFO("Destroying entity {}", id);
                mono_gchandle_free(it.second->mHandle);
                // erase it from the map
                mEntityInstances.erase(it.first);
                break;
            }
        }

        for (auto it = entityAdded.begin(); it != entityAdded.end(); ++it)
        {
            if (*it == id)
            {
                entityAdded.erase(it);
                break;
            }
        }
    }

    void ScriptSystem::Init()
    {
       InitMono();

       ScriptFunctions::RegisterFunctions();

       LoadEntityClasses();

       ScriptFunctions::RegisterComponents();

       // PrintAssemblyTypes(mCoreAssembly);
        // retrieve the main Entity class
       mEntityClass = ScriptClass("Carmicah", "Entity");

    }

    void ScriptSystem::LogMonoHeapSize()
    {
        // Retrieve the current size of the managed heap in bytes
        size_t heapSize = mono_gc_get_heap_size();

        // Convert to KB or MB if needed
        std::cout << "Current Mono managed heap size: " << heapSize << " bytes ("
            << heapSize / 1024 << " KB)" << std::endl;
    }

    void ScriptSystem::CleanUp()
    {
        if (mAppDomain)
        {
            // Switch back to root domain to allow unloading
            mono_domain_set(mRootDomain, false);

            // Unload the application domain
            mono_domain_unload(mAppDomain);
            mAppDomain = nullptr;
        }

        // Force garbage collection and wait for finalizers in managed code
        //mono_gc_collect(mono_gc_max_generation());
        // LogMonoHeapSize();

        if (mRootDomain)
        {
            // Perform JIT cleanup on the root domain
            mono_jit_cleanup(mRootDomain);
            mRootDomain = nullptr;
        }


        mCoreAssemblyImage = nullptr;  // Clear any references to images, assemblies
        mCoreAssembly = nullptr;

        
        // Optional: small delay to ensure Mono completes cleanup
       // std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    void ScriptSystem::InitMono()
    {
        // while (true) {};
        mono_set_assemblies_path("../Dependencies/bin");

        mRootDomain = mono_jit_init("CarmicahJITRuntime");
        if (mRootDomain == nullptr)
        {
            CM_CORE_ERROR("Unable to init mono");
            assert("Failed to init mono jit");
            return;
        }

        //mRootDomain = rootDomain;

        LoadMonoAssembly("../CarmicahScriptCore/CarmicahScriptCore.dll");

        //MonoImage* image = mono_assembly_get_image(mCoreAssembly);
        //MonoClass* monoClass = mono_class_from_name(image, "Carmicah", "Main");
        //MonoObject* classInstance = mono_object_new(mAppDomain, monoClass);

        //mono_runtime_object_init(classInstance);
    }

    /// <summary>
    /// Load a file into an array of bytes that can be passed to Mono directly
    /// </summary>
    /// <param name="filepath">File path to read from</param>
    /// <param name="outSize"> output file</param>
    /// <returns></returns>
    char* ScriptSystem::ReadBytes(const std::string& filepath, uint32_t* outSize)
    {
        std::filesystem::path directoryPath = filepath;
        if (std::filesystem::is_regular_file(directoryPath))
        {
            CM_CORE_INFO("Test");
        }

        std::ifstream stream(directoryPath, std::ios::binary | std::ios::ate);

        if (!stream)
        {
            // Failed to open the file
           // while (true) {};
            return nullptr;
        }
       // while (true) {};

        std::streampos end = stream.tellg();
        stream.seekg(0, std::ios::beg);
        uint32_t size = (uint32_t)(end - stream.tellg());

        if (size == 0)
        {
            // File is empty
            return nullptr;
        }

        char* buffer = new char[size];
        stream.read((char*)buffer, size);
        stream.close();

        *outSize = size;
        return buffer;
    }

    void ScriptSystem::LoadMonoAssembly(const std::string& assemblyPath)
    {
        // Create an app domain
        mAppDomain = mono_domain_create_appdomain("CarmicahAppDomain", nullptr);
        mono_domain_set(mAppDomain, true);
        mCoreAssembly = LoadCSharpAssembly(assemblyPath);
        if (mCoreAssembly == nullptr)
        {
            // assert smth here HasEntityClass
            CM_CORE_ERROR("Unable to load core assembly");
            assert("Unable to load");
        }

        mCoreAssemblyImage = mono_assembly_get_image(mCoreAssembly);
        if (mCoreAssemblyImage == nullptr)
        {
            // assert smth here 
            CM_CORE_ERROR("Unable to load core assembly image");
            assert("Unable to load");
        }
    }

    MonoAssembly* ScriptSystem::LoadCSharpAssembly(const std::string& assemblyPath)
    {
        uint32_t fileSize = 0;
        char* fileData = ReadBytes(assemblyPath, &fileSize);

        // NOTE: We can't use this image for anything other than loading the assembly because this image doesn't have a reference to the assembly
        MonoImageOpenStatus status;
        MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);

        if (status != MONO_IMAGE_OK)
        {
            // Don't forget to free the file data
            delete[] fileData;

            const char* errorMessage = mono_image_strerror(status);
            CM_CORE_ERROR(errorMessage);
            assert(errorMessage);
            // Log some error message using the errorMessage data
            return nullptr;
        }

        MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
        mono_image_close(image);

        // Don't forget to free the file data
        delete[] fileData;

        return assembly;

       // return nullptr;
    }

    void ScriptSystem::PrintAssemblyTypes(MonoAssembly* assembly)
    {
        MonoImage* image = mono_assembly_get_image(assembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
            UNUSED(name);
            UNUSED(nameSpace);
            //printf("%s.%s\n", nameSpace, name);
        }
    }

    bool ScriptSystem::HasEntityClass(std::string scriptName)
    {
        return mEntityClasses.find(scriptName) != mEntityClasses.end();
    }

    void ScriptSystem::OnStart()
    {
        // Loop through all entity instances
        for (const auto& [id, scriptRef] : mEntityInstances)
        {
            scriptRef->InvokeOnConstruct(id);
            scriptRef->InvokeOnCreate();
        }
    }

    void ScriptSystem::OnUpdate(float dt)
    {
        // Loop through all entity instances
        for (const auto& [id, scriptRef] : mEntityInstances)
        {
            scriptRef->InvokeOnUpdate(dt);
        }
    }

    void ScriptSystem::OnFixedUpdate(float dt)
    {
        // Loop through all entity instances
        for (const auto& [id, scriptRef] : mEntityInstances)
        {
            scriptRef->InvokeOnFixedUpdate(dt);
        }
    }

    void ScriptSystem::UpdateScripts()
    {
        for (auto entity = entityAdded.begin(); entity != entityAdded.end(); ++entity)
        {
            if (mEntityInstances.count(*entity) == 0)
            {
                Script& scriptComponent = ComponentManager::GetInstance()->GetComponent<Script>(*entity);
                if (HasEntityClass(scriptComponent.scriptName)) // Technically dont have to check IMGUI only allows for entity classes to be picked
                {
                    std::shared_ptr<ScriptObject> scriptObj = std::make_shared<ScriptObject>(mEntityClasses[scriptComponent.scriptName], *entity);
                    //  scriptRef->SetUpEntity(id); // Instantiate and set up the method handling
                    CM_CORE_INFO("Setting up a new script");

                    mEntityInstances[*entity] = scriptObj;
                    UpdateScriptComponent(*entity);
                    entityAdded.erase(entity);
                    break;
                   // entity = entityAdded.begin();
                }
            }
        }

        // TODO: See if theres a less iterative way to do this instead of looping it in update
      //  for (auto& id : mEntitiesSet)
      //  {
            // new entity added
          
            // it already exist, check if the script was changed
            // this should be done through messaging from IMGUI side if the script was changed on an object
            // but for now we do it here
           // else
          //  {
                // Comment out for now cause i dont think we'll be changign scripts that much in editor mode
                // but NOTE: If we do allow that then it wont work until i fix this part
                
                //Script& scriptComponent = ComponentManager::GetInstance()->GetComponent<Script>(id);
                //if (HasEntityClass(scriptComponent.scriptName))
                //{
                //    std::shared_ptr<ScriptObject> scriptObj = std::make_shared<ScriptObject>(mEntityClasses[scriptComponent.scriptName], id);
                //    //  scriptRef->SetUpEntity(id); // Instantiate and set up the method handling
                //    mEntityInstances[id] = scriptObj;
                //}
          //  }
      //  }
    }

    void ScriptSystem::OnEnd()
    {
        mEntityInstances.clear();
    }

    void ScriptSystem::UpdateScriptVariables(Entity entity)
    {
        Script& scriptComponent = ComponentManager::GetInstance()->GetComponent<Script>(entity);
        auto& scriptRef = mEntityInstances[entity];
        const auto& fields = scriptRef->GetScriptClass()->mFields;
        for (const auto& it : fields)
        {
            if (scriptComponent.scriptableFieldMap.count(it.first) != 0)
            {
                if (it.second.mType == ScriptFieldType::String)
                {
                    std::string str = std::get<std::string>(scriptComponent.scriptableFieldMap[it.first]);
                    scriptRef->SetFieldValue<std::string>(it.second.mName, str);
                }
                else
                {
                    scriptRef->SetFieldValue(it.second.mName.c_str(), scriptComponent.scriptableFieldMap[it.first]);
                }
            }

        }
    }

    void ScriptSystem::UpdateScriptComponent(Entity entity)
    {
        Script& scriptComponent = ComponentManager::GetInstance()->GetComponent<Script>(entity);
        if (mEntityInstances.count(entity) > 0) // Technically dont have to check IMGUI only allows for entity classes to be picked
        {
            auto& scriptRef = mEntityInstances[entity];
            const auto& fields = scriptRef->GetScriptClass()->mFields;
            scriptComponent.scriptableFieldMap.clear();
            //Script::variantVar var;
            for (const auto& it : fields)
            {

                if (it.second.mType == ScriptFieldType::Float)
                {
                    float var = scriptRef->GetFieldValue<float>(it.second.mName);
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
                else if (it.second.mType == ScriptFieldType::Bool)
                {
                    bool var = scriptRef->GetFieldValue<bool>(it.second.mName);
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
                else if (it.second.mType == ScriptFieldType::String)
                {
                    std::string var = scriptRef->GetFieldValue<std::string>(it.second.mName);
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
                else if (it.second.mType == ScriptFieldType::Int)
                {
                    int var = scriptRef->GetFieldValue<int>(it.second.mName);
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
            }
        }
    }

    void ScriptSystem::UpdateScriptPrefabComponent(Script& scriptComponent)
    {
        auto& scriptRef = mEntityClasses[scriptComponent.scriptName];
        const auto& fields = scriptRef->mFields;

        scriptComponent.scriptableFieldMap.clear();

        //Script::variantVar var;
        for (const auto& it : fields)
        {
            if (it.second.mType == ScriptFieldType::Float)
            {
                float var{};
                scriptComponent.scriptableFieldMap[it.first] = var;
            }
            else if (it.second.mType == ScriptFieldType::Bool)
            {
                bool var{};
                scriptComponent.scriptableFieldMap[it.first] = var;
            }
            else if (it.second.mType == ScriptFieldType::String)
            {
                std::string var{};
                scriptComponent.scriptableFieldMap[it.first] = var;
            }
            else if (it.second.mType == ScriptFieldType::Int)
            {
                int var{};
                scriptComponent.scriptableFieldMap[it.first] = var;
            }
        }
    }

    //void ScriptSystem::UpdateAllPrefabScriptComponents()
    //{

    //}

    void ScriptSystem::UpdateExistingPrefabScript(Script& scriptComponent)
    {
     //   Script& scriptComponent = ComponentManager::GetInstance()->GetComponent<Script>(entity);
        auto& scriptRef = mEntityClasses[scriptComponent.scriptName];
        const auto& fields = scriptRef->mFields;

        //check if any variables were removed
        // loop through every variable in the component's current map
        for (auto it = scriptComponent.scriptableFieldMap.begin(); it != scriptComponent.scriptableFieldMap.end();)
        {
            // it is no longer in that entity class
            if (fields.count(it->first) == 0)
            {
                it = scriptComponent.scriptableFieldMap.erase(it); // erasing will return the next iterator already
            }
            else
            {
                // ++ if got no issue
                it++;
            }
        }

        // now cheeck for any variables that isnt in the component but is new in the script
        for (const auto& it : fields)
        {
            // if it doesnt exist now
            if (scriptComponent.scriptableFieldMap.count(it.first) == 0)
            {
                // add it in

                if (it.second.mType == ScriptFieldType::Float)
                {
                    float var{};
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
                else if (it.second.mType == ScriptFieldType::Bool)
                {
                    bool var{};
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
                else if (it.second.mType == ScriptFieldType::String)
                {
                    std::string var{};
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
                else if (it.second.mType == ScriptFieldType::Int)
                {
                    int var{};
                    scriptComponent.scriptableFieldMap[it.first] = var;
                }
            }
        }

    }

    void ScriptSystem::EntityAdded(Entity entity)
    {
      
        if (mEntityInstances.count(entity) == 0)
        {
            Script& scriptComponent = ComponentManager::GetInstance()->GetComponent<Script>(entity);
            if (HasEntityClass(scriptComponent.scriptName)) // Technically dont have to check IMGUI only allows for entity classes to be picked
            {
                std::shared_ptr<ScriptObject> scriptObj = std::make_shared<ScriptObject>(mEntityClasses[scriptComponent.scriptName], entity);
                //  scriptRef->SetUpEntity(id); // Instantiate and set up the method handling
                mEntityInstances[entity] = scriptObj;
                // update the variables in the script reference
                UpdateScriptVariables(entity);
               
                // redundant call atm ill remove it once everything is done
                UpdateScriptComponent(entity);

                // If its in runtime mode, then call the onstart and on create
                auto gameSystem = SystemManager::GetInstance()->GetSystem<SceneSystem>();
                if (gameSystem->mRuntime && gameSystem->mNextState == RUNTIME)
                {
                    //CM_CORE_INFO("Invoking on create");
                    mEntityInstances[entity]->InvokeOnConstruct(entity);
                    mEntityInstances[entity]->InvokeOnCreate();
                }
                //CM_CORE_INFO("New script instance")
                // entity = entityAdded.begin();
            }
            // if no script is assigned yet (i.e in editor mode, if a script is attached, it wouldnt have one added by default
            else
            {
                entityAdded.push_back(entity);
            }
        }

    }

    /// <summary>
    /// // If entity has it's scripting component removed 
    /// </summary>
    /// <param name="entity">Entity being removed</param>
    void ScriptSystem::EntityRemoved(Entity entity)
    {
        for (auto& it : mEntityInstances)
        {
            if (it.first == entity)
            {
                // erase it from the map
                mEntityInstances.erase(it.first);
                break;
            }
        }

        // Check if the obj is in the entities to add
        for (auto it = entityAdded.begin(); it != entityAdded.end(); ++it)
        {
            if (*it == entity)
            {
                entityAdded.erase(it);
                break;
            }
        }
    }

    void ScriptSystem::LoadEntityClasses()
    {
        // clear the map before using it
        mEntityClasses.clear();

        MonoImage* image = mono_assembly_get_image(mCoreAssembly);
        const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
        int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);
        MonoClass* entityClass = mono_class_from_name(image, "Carmicah", "Entity");

        for (int32_t i = 0; i < numTypes; i++)
        {
            uint32_t cols[MONO_TYPEDEF_SIZE];
            mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

            const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
            const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

            MonoClass* monoClass = mono_class_from_name(image, nameSpace, name);

            // don't reload entity class
            if (monoClass == entityClass) continue;

            std::string className;
            if (strlen(nameSpace))
            {
                className = fmt::format("{}.{}", nameSpace, name);
            }
            else
                className = name;

            bool isEntityScript = mono_class_is_subclass_of(monoClass, entityClass, false);
            if (isEntityScript)
            {
                std::shared_ptr<ScriptClass> script = std::make_shared<ScriptClass>(nameSpace, name);
                mEntityClasses[className] = script;

                MonoClass* currentClass = monoClass;
                while (currentClass)
                {
                    // get all the fields from the c# script (i.e variables from c# script side)
                    void* iterator = nullptr;
                    while (MonoClassField* field = mono_class_get_fields(currentClass, &iterator))
                    {
                        std::string fieldName = mono_field_get_name(field);
                        // Only access public variables from the mono class
                        if (mono_field_get_flags(field) & FIELD_ATTRIBUTE_PUBLIC)
                        {
                            MonoType* type = mono_field_get_type(field);
                            ScriptFieldType fieldType = GetScriptFieldType(type);
                            variantVar defaultValue;

                            // Store it in the script's field map
                            script->mFields[fieldName] = { fieldType, fieldName, field, defaultValue};
                        }
                    }

                    currentClass = mono_class_get_parent(currentClass);
                    if (currentClass == entityClass)
                        break;
                }
            }

            //printf("%s.%s\n", nameSpace, name);

           
        }

    }

    variantVar ScriptSystem::ExtractDefaultValue(MonoObject* valObj, ScriptFieldType type)
    {
        if (!valObj) return {}; // Default-constructs a variant

        switch (type)
        {
        case ScriptFieldType::Float:
            return *(float*)mono_object_unbox(valObj);
        case ScriptFieldType::Int:
            return *(int*)mono_object_unbox(valObj);
        case ScriptFieldType::Bool:
            return *(bool*)mono_object_unbox(valObj);
        case ScriptFieldType::String:
        {
            MonoString* monoStr = (MonoString*)valObj;
            char* utf8Str = mono_string_to_utf8(monoStr);
            std::string result(utf8Str);
            mono_free(utf8Str);

            return result;

        }
        default:
            return {}; // Default-initialize variant
        }
    }

    ScriptFieldType ScriptSystem::GetScriptFieldType(MonoType* type)
    {
        std::string name = mono_type_get_name(type);
        // If the name exist in our field type map
        if (sFieldTypeMap.count(name) != 0)
        {
            auto iter = sFieldTypeMap.find(name);
            return iter->second;
        }

        return ScriptFieldType::None;
    }

    std::shared_ptr<ScriptObject> ScriptSystem::GetScriptInstance(unsigned int entityID)
    {
        if (mEntityInstances.count(entityID) == 0)
        {
            return nullptr;
        }

        return mEntityInstances[entityID];
    }

    void ScriptSystem::ReceiveMessage(Message* msg)
    {
        auto gameSystem = SystemManager::GetInstance()->GetSystem<SceneSystem>();
        if (!gameSystem->mRuntime && gameSystem->mNextState != RUNTIME)
        {
            return;
        }
        // Button entity was clicked
        if (msg->mMsgType == MSG_ONCLICK)
        {
            auto castedMsg = dynamic_cast<OnMouseMsg*>(msg);

            // If it has a script instance
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnClick();
            }
        }
        else if (msg->mMsgType == MSG_ENTITYCOLLIDED)
        {
            auto castedMsg = dynamic_cast<EntityCollidedMessage*>(msg);
            //CM_CORE_INFO("Entity id in ScriptSys {}", castedMsg->mEntityID);w
            if (castedMsg->mCollidedType == ON_COLLIDE)
            {
                if (mEntityInstances.count(castedMsg->mEntityID))
                {
                    mEntityInstances[castedMsg->mEntityID]->InvokeOnCollide(castedMsg->mCollidedEntity);
                }
            }
            else if (castedMsg->mCollidedType == TRIGGER_ENTER)
            {
                if (mEntityInstances.count(castedMsg->mEntityID))
                {
                    mEntityInstances[castedMsg->mEntityID]->InvokeOnTriggerEnter(castedMsg->mCollidedEntity);
                }
            }
            else if (castedMsg->mCollidedType == TRIGGER_STAY)
            {
                if (mEntityInstances.count(castedMsg->mEntityID))
                {
                    mEntityInstances[castedMsg->mEntityID]->InvokeOnTriggerStay(castedMsg->mCollidedEntity);
                }

            }
            else if (castedMsg->mCollidedType == TRIGGER_EXIT)
            {
                if (mEntityInstances.count(castedMsg->mEntityID))
                {
                    //CM_CORE_INFO("Trigger Exit called");
                    mEntityInstances[castedMsg->mEntityID]->InvokeOnTriggerExit();
                }

            }

        }
        else if (msg->mMsgType == MSG_MOUSEENTER)
        {
            auto castedMsg = dynamic_cast<OnMouseMsg*>(msg);
           // CM_CORE_INFO("Enter " + std::to_string(castedMsg->entity));
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnMouseEnter();
            }

        }
        else if (msg->mMsgType == MSG_MOUSEEXIT)
        {
            auto castedMsg = dynamic_cast<OnMouseMsg*>(msg);
            //CM_CORE_INFO("Exit " + std::to_string(castedMsg->entity));
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnMouseExit();
            }

        }
        else if (msg->mMsgType == MSG_MOUSEHOVER)
        {
            auto castedMsg = dynamic_cast<OnMouseMsg*>(msg);
           // CM_CORE_INFO("Hover " + std::to_string(castedMsg->entity));
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnMouseHover();
            }
        }
        else if (msg->mMsgType == MSG_ENTERSTATE)
        {
            auto castedMsg = dynamic_cast<OnStateEnterMsg*>(msg);
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnStateEnter(castedMsg->stateName);
            }
        }
        else if (msg->mMsgType == MSG_UPDATESTATE)
        {
            auto castedMsg = dynamic_cast<OnStateUpdateMsg*>(msg);
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnStateUpdate(castedMsg->stateName, castedMsg->dt);
            }

        }
        else if (msg->mMsgType == MSG_EXITSTATE)
        {
            auto castedMsg = dynamic_cast<OnStateExitMsg*>(msg);
            if (mEntityInstances.count(castedMsg->entity))
            {
                mEntityInstances[castedMsg->entity]->InvokeOnStateExit(castedMsg->stateName);
            }

        }
    }
}