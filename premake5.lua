workspace "FusionLab"

    architecture "x86"

    configurations { "Debug", "Release" }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "FusionLab"

    kind "ConsoleApp"
    
    language "C++"
    cppdialect "C++20"

    targetdir ("./bin/".. outputdir.. "/%{prj.name}")
    objdir ("./bin-int/".. outputdir.. "/%{prj.name}") 

    files { "./%{prj.name}/src/**.cpp", "./%{prj.name}/src/**.h" }

    includedirs { "./vendor/SDL2/include", "./vendor/SDL2_image/include", "./%{prj.name}/src" }

    libdirs { "./vendor/SDL2/lib/x86", "./vendor/SDL2_image/lib/x86" }

    links { "SDL2", "SDL2main", "SDL2_image" }

    postbuildcommands { ("{COPY} ./vendor/SDL2/lib/x86/SDL2.dll ./bin/".. outputdir.. "/%{prj.name}"), ("{COPY} ./vendor/SDL2_image/lib/x86/SDL2_image.dll ./bin/".. outputdir.. "/%{prj.name}") }

    staticruntime "On"

    systemversion "latest"

    filter "configurations:Debug"
      defines "FL_DEBUG"
      symbols "On"
    
    filter "configurations:Release"
      defines "FL_RELEASE"
      optimize "On"