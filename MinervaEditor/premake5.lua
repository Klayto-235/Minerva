project "MinervaEditor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++20"
	staticruntime "On"
	systemversion "latest"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.h",
		"src/**.cpp"
	}

	includedirs
	{
		"%{includeDir.spdlog}",
		"%{includeDir.glm}",
		"%{includeDir.imgui}",
		"%{includeDir.entt}",
		"%{wks.location}/Minerva/src",
		"src"
	}

	links
	{
		"Minerva"
	}

	filter "system:windows"
		defines
		{
			"MN_PLATFORM_WINDOWS"
		}
	
	filter "system:linux"
		defines
		{
			"MN_PLATFORM_LINUX"
		}

		links
		{
			"GLFW",
			"glad",
			"imgui"
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