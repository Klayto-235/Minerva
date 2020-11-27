project "glad"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/include/glad/glad.h",
		"%{prj.name}/include/KHR/khrplatform.h",
		"%{prj.name}/src/glad.c",
	}

	includedirs
	{
		"glad/include"
	}

	filter "system:windows"
		systemversion "latest"

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release or Dist"
		runtime "Release"
		symbols "Off"
		optimize "Full"