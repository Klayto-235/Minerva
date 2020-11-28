workspace "Minerva"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	startproject "Sandbox"

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	includeDir = {}
	includeDir["spdlog"] = "Minerva/vendor/spdlog/include"
	includeDir["GLFW"] = "Minerva/vendor/GLFW/include"
	includeDir["glad"] = "Minerva/vendor/glad/include"
	includeDir["imgui"] = "Minerva/vendor/imgui"
	includeDir["glm"] = "Minerva/vendor/glm"

	group "Dependencies"
		include "Minerva/vendor/premake_GLFW.lua"
		include "Minerva/vendor/premake_glad.lua"
		include "Minerva/vendor/premake_imgui.lua"
	group ""

	project "Minerva"
		location "Minerva"
		kind "SharedLib"
		language "C++"
		staticruntime "off"
	
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
			"%{includeDir.spdlog}",
			"%{includeDir.GLFW}",
			"%{includeDir.glad}",
			"%{includeDir.imgui}",
			"%{includeDir.glm}"
		}
	
		links
		{
			"GLFW",
			"glad",
			"opengl32.lib",
			"imgui"
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
				("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/Sandbox\"")
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
		staticruntime "off"
	
		targetdir ("bin/" .. outputdir .. "/%{prj.name}")
		objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

		files
		{
			"%{prj.name}/src/**.h",
			"%{prj.name}/src/**.cpp"
		}

		includedirs
		{
			"%{includeDir.spdlog}",
			"%{includeDir.glm}",
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