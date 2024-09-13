-- premake5.lua
workspace "CarmicahEngine"
    configurations { "Debug", "Release" }
    architecture "x64"
    system "windows"

outputdir = "%{cfg.architecture}"

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

    --libdirs
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