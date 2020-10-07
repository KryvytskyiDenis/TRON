workspace "Tron"
    architecture "x64"
    startproject "Tron"

    configurations
    {
        "Debug",
        "Release",
    }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/Tron/dependencies/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/Tron/dependencies/Glad/include"
IncludeDir["glm"] = "%{wks.location}/Tron/dependencies/glm"
IncludeDir["spdlog"] = "%{wks.location}/Tron/dependencies/spdlog/include"

include "Tron/dependencies/GLFW"
include "Tron/dependencies/Glad"

project "Tron"
    location "Tron"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "trpch.h"
    pchsource "%{prj.name}/src/trpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/dependencies/glm/glm/**.hpp",
        "%{prj.name}/dependencies/glm/glm/**.inl"
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.spdlog}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    defines
    {
        "GLFW_INCLUDE_NONE"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Debug"
        defines "TR_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "TR_RELEASE"
        runtime "Release"
        optimize "on"