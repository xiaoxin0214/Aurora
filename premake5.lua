workspace "Aurora"
	architecture "x64"
	configurations
	{
		"Debug",
		"Release",
		"Distribution"
	}

startproject "Editor"

VULKAN_SDK = os.getenv("VULKAN_SDK")

outputdir="%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
IncludeDir={}
IncludeDir["GLFW"]="Aurora/vendor/GLFW/include"
IncludeDir["GLAD"]="Aurora/vendor/GLAD/include"
IncludeDir["ImGui"]="Aurora/vendor/imgui"
IncludeDir["GLM"]="Aurora/vendor/glm/glm"
IncludeDir["stbimage"]="Aurora/vendor/stb_image"
IncludeDir["Optick"] = "Aurora/vendor/optick/src"
IncludeDir["entt"] = "Aurora/vendor/entt"
IncludeDir["Yaml"] = "Aurora/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Aurora/vendor/ImGuizmo"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir={}
LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"

Library = {}

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"



group "Dependencies"
include "Aurora/vendor/GLFW"
include "Aurora/vendor/GLAD"
include "Aurora/vendor/imgui"
include "Aurora/vendor/optick"
include "Aurora/vendor/yaml-cpp"

group ""
project "Aurora"
	location "Aurora"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"
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
		"%{prj.name}/vendor/stb_image/stb_image.cpp",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
		"%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp"
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
		"%{IncludeDir.Optick}",
		"%{IncludeDir.entt}",
		"%{IncludeDir.Yaml}",
		"%{IncludeDir.ImGuizmo}",
		"%{IncludeDir.VulkanSDK}"
	}

	links
	{
		"GLFW",
		"GLAD",
		"ImGui",
		"Optick",
		"Yaml",
		"opengl32.lib"
	}

	filter "files:Aurora/vendor/ImGuizmo/**.cpp"
	flags {"NoPCH"}

	filter "system:windows"
		systemversion "latest"
		defines
		{
			"AURORA_PLATFORM_WINDOWS",
			"AURORA_BUILD_DLL",
			"GLFW_INCLUDE_NONE",
			"YAML_CPP_STATIC_DEFINE",
			"_CRT_SECURE_NO_WARNINGS"
		}

	filter "configurations:Debug"
		defines
		{
			"AURORA_DEBUG",
		}
		links
		{
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}
		symbols "on"
		runtime "Debug"

	filter "configurations:Release"
		defines
		{
			"AURORA_RELEASE"
		}
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
		optimize "on"
		runtime "Release"


	filter "configurations:Distribution"
		defines
		{
			"AURORA_DISTRIBUTION"
		}
		links
		{
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}
		optimize "on"
		runtime "Release"

project "Editor"
		location "Editor"
		kind "ConsoleApp"
		language "C++"
		cppdialect "C++17"
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
			"%{IncludeDir.GLM}",
			"%{IncludeDir.ImGui}",
			"%{IncludeDir.entt}",
			"%{IncludeDir.ImGuizmo}",
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
		"%{IncludeDir.GLM}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.entt}",
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