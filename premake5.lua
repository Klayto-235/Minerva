workspace "Minerva"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Minerva/vendor/GLFW/include"
IncludeDir["glad"] = "Minerva/vendor/glad/include"
IncludeDir["spdlog"] = "Minerva/vendor/spdlog/include",

include "Minerva/vendor/premake_GLFW.lua"
include "Minerva/vendor/premake_glad.lua"

project "Minerva"
	location "Minerva"
	kind "SharedLib"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
	
	pchheader "mnpch.h"
	pchsource "Minerva/src/mnpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.glad}"
	}
	
	links
	{
		"GLFW",
		"glad",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS",
			"MN_BUILD_DLL"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		}
		
	filter "configurations:Debug"
		defines 
		{
			"MN_DEBUG",
			"MN_ENABLE_ASSERTS",
			"MN_ENABLE_DEBUG_CODE"
		}
		runtime "Debug"
		symbols "On"
		optimize "Off"
	
	filter "configurations:Release"
		defines
		{
			"MN_RELEASE",
			"MN_ENABLE_DEBUG_CODE"
		}
		runtime "Release"
		symbols "Off"
		optimize "Full"

	filter "configurations:Dist"
		defines "MN_DIST"
		runtime "Release"
		symbols "Off"
		optimize "Full"

project "Sandbox"
	location "Sandbox"
	kind "ConsoleApp"
	language "C++"
	
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{IncludeDir.spdlog}",
		"Minerva/src"
	}

	links
	{
		"Minerva"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines 
		{
			"MN_DEBUG",
			"MN_ENABLE_ASSERTS",
			"MN_ENABLE_DEBUG_CODE"
		}
		runtime "Debug"
		symbols "On"
		optimize "Off"
	
	filter "configurations:Release"
		defines
		{
			"MN_RELEASE",
			"MN_ENABLE_DEBUG_CODE"
		}
		runtime "Release"
		symbols "Off"
		optimize "Full"

	filter "configurations:Dist"
		defines "MN_DIST"
		runtime "Release"
		symbols "Off"
		optimize "Full"