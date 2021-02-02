workspace "Minerva"
	architecture "x86_64"
	startproject "MinervaEditor"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	flags
	{
		"MultiProcessorCompile"
	}

	outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

	includeDir = {}
	includeDir["spdlog"] = "%{wks.location}/Minerva/vendor/spdlog/include"
	includeDir["GLFW"] = "%{wks.location}/Minerva/vendor/GLFW/include"
	includeDir["glad"] = "%{wks.location}/Minerva/vendor/glad/include"
	includeDir["imgui"] = "%{wks.location}/Minerva/vendor/imgui"
	includeDir["glm"] = "%{wks.location}/Minerva/vendor/glm"
	includeDir["stb_image"] = "%{wks.location}/Minerva/vendor/stb_image"
	includeDir["entt"] = "%{wks.location}/Minerva/vendor/EnTT"

	group "Dependencies"
		include "vendor/premake"
		include "Minerva/vendor/premake_GLFW.lua"
		include "Minerva/vendor/premake_glad.lua"
		include "Minerva/vendor/premake_imgui.lua"
	group ""

	include "Minerva"
	include "Sandbox"
	include "MinervaEditor"