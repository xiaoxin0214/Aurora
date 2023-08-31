workspace "Aurora"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

startproject "Editor"

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir={}
IncludeDir["GLFW"]="Aurora/vendor/GLFW/include"
IncludeDir["GLAD"]="Aurora/vendor/GLAD/include"
IncludeDir["ImGui"]="Aurora/vendor/imgui"
IncludeDir["GLM"]="Aurora/vendor/glm/glm"
IncludeDir["stbimage"]="Aurora/vendor/stb_image"
IncludeDir["Optick"] = "Aurora/vendor/optick/src"

group "Dependencies"
include "Aurora/vendor/GLFW"
include "Aurora/vendor/GLAD"
include "Aurora/vendor/imgui"
include "Aurora/vendor/optick"

group ""
project "Aurora"
	location "Aurora"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.h"
	pchsource "Aurora/src/pch.cpp"

	files
	{
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.cpp",
		"%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
		"%{prj.name}/vendor/stb_image/stb_image.h",
		"%{prj.name}/vendor/stb_image/stb_image.cpp"
	}

	includedirs
	{
		"%{prj.name}/vendor/spdlog/include",
		"%{prj.name}/src",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.GLAD}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.stbimage}",
		"%{IncludeDir.Optick}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"Optick",
		"opengl32.lib"
	}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"AURORA_PLATFORM_WINDOWS",
			"AURORA_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines
		{
			"AURORA_DEBUG",
		}
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines
		{
			"AURORA_RELEASE"
		}
		optimize "on"
		runtime "Release"


	filter "configurations:Distribution"
		defines
		{
			"AURORA_DISTRIBUTION"
		}
		optimize "on"
		runtime "Release"

project "Editor"
		location "Editor"
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
			"Aurora/vendor/spdlog/include",
			"%{IncludeDir.GLM}",
			"%{IncludeDir.ImGui}",
			"Aurora/src "
		}
	
		links
		{
			"Aurora"
		}
	
		filter "system:windows"
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

project "SandBox"
	location "SandBox"
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
		"Aurora/vendor/spdlog/include",
		"%{IncludeDir.GLM}",
		"%{IncludeDir.ImGui}",
		"Aurora/src "
	}

	links
	{
		"Aurora"
	}

	filter "system:windows"
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