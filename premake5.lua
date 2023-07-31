workspace "Aurora"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

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
		staticruntime "On"
		systemversion "latest"
		defines
		{
			"AURORA_PLATFORM_WINDOWS",
			"AURORA_BUILD_DLL",
			"GLFW_INCLUDE_NONE"
		}

		postbuildcommands
		{
			("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/SandBox")
		}

	filter "configurations:Debug"
		defines
		{
			"AURORA_DEBUG",
			"AURORA_ENABLE_ASSERT"
		}
		symbols "On"
		buildoptions "/MDd"

	filter "configurations:Release"
		defines
		{
			"AURORA_RELEASE"
		}
		optimize "On"
		buildoptions "/MD"


	filter "configurations:Distribution"
		defines
		{
			"AURORA_DISTRIBUTION"
		}
		optimize "On"
		buildoptions "/MD"

project "SandBox"
	location "SandBox"
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
		"Aurora/vendor/spdlog/include",
		"Aurora/src "
	}

	links
	{
		"Aurora"
	}

	filter "system:windows"
		cppdialect "C++17"
		staticruntime "On"
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
		buildoptions "/MDd"

	filter "configurations:Release"
		defines
		{
			"AURORA_RELEASE"
		}
		optimize "On"
		buildoptions "/MD"

	filter "configurations:Distribution"
		defines
		{
			"AURORA_DISTRIBUTION"
		}
		optimize "On"
		buildoptions "/MD"