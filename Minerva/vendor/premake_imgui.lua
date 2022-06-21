project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	systemversion "latest"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/*.cpp",
		"%{prj.name}/*.h"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"
		
	filter "configurations:Release or Dist"
		runtime "Release"
		symbols "Off"
		optimize "Full"