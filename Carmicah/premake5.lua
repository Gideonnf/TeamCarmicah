workspace "Carmicah"
	architecture "x64"
	configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Carmicah/Dependencies/GLFW/include"

include "Carmicah/Dependencies/GLFW"

project "Carmicah"
	location "Carmicah"
	kind "StaticLib"
	language "C++"
	-- any files created will be put in bin/outputdir/Carmicah
	-- buildcfg can be either debug or Release
	-- architecture only x64
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	-- Add the pre compile header into carmicah sln
	pchheader "pch.h"
	pchsource "Carmicah/source/pch.cpp"

	files 
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"Carmicah/source", -- so we dont have to source/file.h everytime to include
		"%{IncludeDir.GLFW}"
		-- nth else
		-- if we include any libraries, add it here
	}

	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "10.0"

		defines
		{
			"CM_PLATFORM_WINDOWS"
		}

	-- Add a CM_DEBUG preprocessor if its in debug mode
	filter "configurations:Debug"
		defines "CM_DEBUG"
		symbols "On"

	-- Add a CM_DEBUG preprocessor if its in release mode
	filter "configurations:Release"
		defines "CM_RELEASE"
		symbols "On"

project "CarmicahApp"
	location "CarmicahApp"
	kind "ConsoleApp"

	language "C++"
	-- any files created will be put in bin/outputdir/Carmicah
	-- buildcfg can be either debug or Release
	-- architecture only x64
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin_int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/source/**.h",
		"%{prj.name}/source/**.cpp"
	}

	includedirs
	{
		"Carmicah/source" -- so we dont have to source/file.h everytime to include
		-- nth else
		-- if we include any libraries, add it here
	}

	links
	{
		"Carmicah"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "10.0"

		defines
		{
			"CM_PLATFORM_WINDOWS"
		}

	-- Add a CM_DEBUG preprocessor if its in debug mode
	filter "configurations:Debug"
		defines "CM_DEBUG"
		symbols "On"

	-- Add a CM_DEBUG preprocessor if its in release mode
	filter "configurations:Release"
		defines "CM_RELEASE"
		symbols "On"

