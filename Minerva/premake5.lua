project "Minerva"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "mnpch.h"
	pchsource "src/mnpch.cpp"

	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"src",
		"%{includeDir.spdlog}",
		"%{includeDir.GLFW}",
		"%{includeDir.glad}",
		"%{includeDir.imgui}",
		"%{includeDir.glm}",
		"%{includeDir.stb_image}",
		"%{includeDir.entt}"
	}

	links
	{
		"GLFW",
		"glad",
		"opengl32.lib",
		"imgui"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"MN_PLATFORM_WINDOWS",
			"GLFW_INCLUDE_NONE"
		}
	
	filter "configurations:Debug"
		defines 
		{
			"MN_DEBUG",
			"MN_ENABLE_ASSERTS",
			"MN_ENABLE_DEBUG_CODE",
			"MN_ENABLE_OPENGL_ERRORS",
			"MN_ENABLE_OPENGL_ASSERTS"
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