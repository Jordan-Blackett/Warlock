workspace "BlackThorn"
	architecture "x64"
	startproject "WhiteThorn"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["GLFW"] = "BlackThorn/vendor/GLFW/include"
IncludeDir["Glad"] = "BlackThorn/vendor/Glad/include"
IncludeDir["ImGui"] = "BlackThorn/vendor/imgui"
IncludeDir["glm"] = "BlackThorn/vendor/glm"
IncludeDir["stb_image"] = "BlackThorn/vendor/stb_image"

group "Dependencies"
	include "BlackThorn/vendor/GLFW"
	include "BlackThorn/vendor/Glad"
	include "BlackThorn/vendor/imgui"

group ""

-- BLACKTHORN
project "BlackThorn"
	location "BlackThorn"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "btpch.h"
	pchsource "BlackThorn/src/btpch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
	}

	defines 
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	includedirs
	{
		"%{prj.name}/src",
		"%{prj.name}/vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
	}

	links 
	{ 
		"wsock32.lib",
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	
	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS",
			"BT_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BT_DIST"
		runtime "Release"
		optimize "on"

-- WHITETHORN
project "WhiteThorn"
	location "WhiteThorn"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"BlackThorn/vendor/spdlog/include",
		"BlackThorn/src",
		"BlackThorn/vendor",
		"%{IncludeDir.glm}"
	}

	links 
	{
		"BlackThorn"
	}

	filter "system:windows"
		systemversion "latest"

		defines
		{
			"BT_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines "BT_DEBUG"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		defines "BT_RELEASE"
		runtime "Release"
		optimize "on"

	filter "configurations:Dist"
		defines "BT_DIST"
		runtime "Release"
		optimize "on"