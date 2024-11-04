#include "pch.h"
#include "ScriptSystem.h"
#include "../log.h"
#include <filesystem>
#include <mono/metadata/mono-gc.h>
#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
namespace Carmicah
{
    ScriptSystem::ScriptSystem()
    {
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

    void ScriptSystem::Init()
    {
       InitMono();
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
        //mono_set_assemblies_path("../Dependencies/bin");

        //mRootDomain = mono_jit_init("CarmicahJITRuntime");
        //if (mRootDomain == nullptr)
        //{
        //    CM_CORE_ERROR("Unable to init mono");
        //    assert("Failed to init mono jit");
        //    return;
        //}

        ////mRootDomain = rootDomain;

        //// Create an app domain
        //mAppDomain = mono_domain_create_appdomain("CarmicahAppDomain", nullptr);
        //mono_domain_set(mAppDomain, true);
        //mCoreAssembly = LoadCSharpAssembly("../CarmicahScriptCore/CarmicahScriptCore.dll");
        //mCoreAssemblyImage = mono_assembly_get_image(mCoreAssembly);

        //PrintAssemblyTypes(mCoreAssembly);

        //mEntityClass = ScriptObject("Carmicah", "Entity");

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
        uint32_t size = end - stream.tellg();

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

            printf("%s.%s\n", nameSpace, name);
        }
    }
}