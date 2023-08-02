workspace "Aurora"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

startproject "SandBox"

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir={}
IncludeDir["GLFW"]="Aurora/vendor/GLFW/include"
IncludeDir["GLAD"]="Aurora/vendor/GLAD/include"
IncludeDir["ImGui"]="Aurora/vendor/imgui"

include "Aurora/vendor/GLFW"
include "Aurora/vendor/GLAD"
include "Aurora/vendor/imgui"


project "Aurora"
	location "Aurora"
	kind "SharedLib"
	language "C++"
	staticruntime "off"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Aurora/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"opengl32.lib"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"AURORA_PLATFORM_WINDOWS",
			"AURORA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/SandBox/\"")
		}

	filter "configurations:Debug"
		defines
		{
			"AURORA_DEBUG",
			"AURORA_ENABLE_ASSERT"
		}
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines
		{
			"AURORA_RELEASE"
		}
		optimize "On"
		runtime "Release"


	filter "configurations:Distribution"
		defines
		{
			"AURORA_DISTRIBUTION"
		}
		optimize "On"
		runtime "Release"

project "SandBox"
	location "SandBox"
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
		"Aurora/vendor/spdlog/include",
		"Aurora/src "
	}

	links
	{
		"Aurora"
	}

	filter "system:windows"
		cppdialect "C++17"
		systemversion "latest"
		defines
		{
			"AURORA_PLATFORM_WINDOWS"
		}

	filter "configurations:Debug"
		defines
		{
			"AURORA_DEBUG"
		}
		symbols "On"
		runtime "Debug"

	filter "configurations:Release"
		defines
		{
			"AURORA_RELEASE"
		}
		optimize "On"
		runtime "Release"

	filter "configurations:Distribution"
		defines
		{
			"AURORA_DISTRIBUTION"
		}
		optimize "On"
		runtime "Release"