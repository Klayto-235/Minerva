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
IncludeDir["spdlog"] = "Minerva/vendor/spdlog/include",

include "Minerva/vendor/premake_GLFW.lua"

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
		"%{IncludeDir.GLFW}"
	}
	
	links
	{
		"GLFW",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
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
		defines "MN_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "MN_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "MN_DIST"
		symbols "On"

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
		staticruntime "On"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS"
		}
		
	filter "configurations:Debug"
		defines "MN_DEBUG"
		symbols "On"
	
	filter "configurations:Release"
		defines "MN_RELEASE"
		symbols "On"

	filter "configurations:Dist"
		defines "MN_DIST"
		symbols "On"