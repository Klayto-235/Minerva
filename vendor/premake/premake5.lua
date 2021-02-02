project "Premake"
	kind "Utility"
	
	targetdir ("output/bin")
	objdir ("!output/bin-int")

	files
	{
		"%{wks.location}/**premake5.lua",
		"%{wks.location}/Minerva/vendor/*.lua"
	}

	postbuildmessage "Regenerating project files with Premake5."
	postbuildcommands
	{
		"%{prj.location}/bin/premake5 %{_ACTION} --file=\"%{wks.location}/premake5.lua\""
	}