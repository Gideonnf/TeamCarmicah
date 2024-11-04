-- premake5.lua
workspace "CarmicahEngine"
    configurations { "Debug", "Release" }
    architecture "x64"
    system "windows"
    startproject "Editor"
    flags "MultiProcessorCompile"
    warnings "Extra"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Carmicah"
    location "Carmicah"
    kind "StaticLib"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    pchheader "pch.h"
    pchsource "Carmicah/source/pch.cpp"

    defines { 
        "_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS", 
        "NOMINMAX"  -- Prevent <windows.h> from defining min/max macros
    }
    files 
    {
        "%{prj.name}/source/**"
    }

    externalincludedirs 
    {
        "Carmicah/source",
        "Dependencies/includes" ,
        "Dependencies/bin"
    }

    libdirs {
        "Dependencies/lib"
    }

    links
    {
        "glfw3.lib",
        "glad",
        "ImGUI",
        "freetype.lib",
        "libmono-static-sgen.lib",
        "Ws2_32.lib",
        "Winmm.lib",
        "Version.lib",
        "Bcrypt.lib"
    }

    buildoptions { "/wd4003" }
    linkoptions { 
        "/ignore:4099",
        "/NODEFAULTLIB:LIBCMT",
        "/ignore:4098"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        defines
        {
            "CM_PLATFORM_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL",
            "STB_IMAGE_IMPLEMENTATION"
        }
        postbuildcommands -- copies dll files to Editor's bin (the exe)
        {
            "{COPYDIR} %[Dependencies/lib/**.dll] %[bin/" .. outputdir .. "/Editor]",
            "{COPYDIR} %[Dependencies/bin/**.dll] %[bin/" .. outputdir .. "/Dependencies/bin]",
            "{COPYDIR} %[Assets/**.**] %[bin/" .. outputdir .. "/Assets]"
        }

    filter "configurations:Debug"
        defines { "CM_DEBUG" }
        symbols "On"
        links 
        {
            "fmodL_vc.lib"
        }

    filter "configurations:Release"
        defines { "CM_RELEASE" }
        optimize "On"
        links 
        {
            "fmod_vc.lib"
        }

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    files 
    {
        "%{prj.name}/source/**"
    }

    externalincludedirs 
    {
        "Carmicah/source" 
    }

    libdirs {
        "Dependencies/lib"
    }

    links
    {
        "Carmicah"
    }

    buildoptions { "/wd4003" }
    linkoptions { 
        "/ignore:4099",
        "/NODEFAULTLIB:LIBCMT",
        "/ignore:4098"
    }
    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        defines
        {
            "CM_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "RELEASE" }
        optimize "On"

-- exists outside of pch.h, since using .c, and pch is pre-compiled
project "glad"
    location "glad"
    kind "StaticLib"
    language "C"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.c" 
    }

    externalincludedirs 
    {
        "Dependencies/includes"
    }

    linkoptions { 
        "/ignore:4099",
        "/NODEFAULTLIB:LIBCMT",
        "/ignore:4098"
    }

    filter "system:windows"
        cdialect "C11"
        systemversion "latest"
        defines
        {
            "CM_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "CM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "CM_RELEASE" }
        optimize "On"

project "ImGUI"
    location "ImGUI"
    kind "StaticLib"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    files 
    {
        "%{prj.name}/src/**.cpp" 
    }

    externalincludedirs 
    {
        "Dependencies/includes/ImGUI",
        "Dependencies/includes"
    }

    libdirs
    {
        "Dependencies/lib"
    }

    linkoptions { 
        "/ignore:4099",
        "/NODEFAULTLIB:LIBCMT",
        "/ignore:4098"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        defines
        {
            "CM_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines { "CM_DEBUG" }
        symbols "On"

    filter "configurations:Release"
        defines { "CM_RELEASE" }
        optimize "On"

project "CarmicahScriptCore"
    location "CarmicahScriptCore"
    kind "SharedLib"
    language "C#"
    dotnetframework "4.7.2"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    files
    {
        "%{prj.name}/Source/**.cs",
        "%{prj.name}/Properties/**.cs"
    }
    
    externalincludedirs 
    {
        "Dependencies/includes"
    }


    libdirs
    {
        "Dependencies/lib"
    }


    filter "configurations:Debug"
        optimize "Off"
        symbols "Default"
    
    filter "configurations:Release"
        optimize "Off"
        symbols "Default"
    
    filter "configurations:Dist"
        optimize "Full"
        symbols "Off"
   