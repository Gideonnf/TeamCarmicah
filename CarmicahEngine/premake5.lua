-- premake5.lua
workspace "CarmicahEngine"
    configurations { "Debug", "Release" }
    architecture "x64"
    system "windows"
    startproject "Editor"

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

project "Carmicah"
    location "Carmicah"
    kind "StaticLib"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    pchheader "pch.h"
    pchsource "Carmicah/source/pch.cpp"

    files 
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/source/**.cpp" 
    }

    includedirs 
    {
        "Carmicah/include",
        "Dependencies/includes" 
    }

    libdirs {
        "Dependencies/lib"
    }

    links
    {
        "glfw3.lib",
        "opengl32.lib",
        "glad",
        "ImGUI"
    }

    filter "system:windows"
        cppdialect "C++17"
        systemversion "latest"
        defines
        {
            "CM_PLATFORM_WINDOWS",
            "GLM_ENABLE_EXPERIMENTAL"
        }
        postbuildcommands -- copies dll files to Editor's bin (the exe)
        {
            "{COPYDIR} %[Dependencies/lib/**.dll] %[bin/" .. outputdir .. "/Editor]",
            "{COPYDIR} %[Assets/Audio/**.**] %[bin/" .. outputdir .. "/Assets/Audio]",
            "{COPYDIR} %[Assets/Shaders/**.**] %[bin/" .. outputdir .. "/Assets/Shaders]"
        }

    filter "configurations:Debug"
        defines { "CM_DEBUG" }
        symbols "On"
        links "fmodL_vc.lib"

    filter "configurations:Release"
        defines { "CM_RELEASE" }
        optimize "On"
        links "fmod_vc.lib"

project "Editor"
    location "Editor"
    kind "ConsoleApp"
    language "C++"
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin/" .. outputdir .. "int/%{prj.name}")

    files 
    {
        "%{prj.name}/include/**.h",
        "%{prj.name}/source/**.cpp" 
    }

    includedirs 
    {
        "Carmicah/include" 
    }

    libdirs {
        "Dependencies/lib"
    }

    links
    {
        "Carmicah"
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

    includedirs 
    {
        "Dependencies/includes"
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

    includedirs 
    {
        "Dependencies/includes/ImGUI",
        "Dependencies/includes"
    }

    libdirs
    {
        "Dependencies/lib"
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
