workspace "Mighty"
  architecture "x64"
  startproject "Mighty"

  configurations
  {
    "Debug",
    "Release",
    "Dist"
  }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glm"] = "Mighty/vendor/glm"
IncludeDir["lua"] = "Mighty/vendor/lua"
IncludeDir["sdl"] = "Mighty/vendor/sdl"

project "Mighty"
  location "Mighty"
  kind "ConsoleApp"
  language "C++"
  cppdialect "C++17"
  staticruntime "on"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

  -- pchheader "mtpch.h"
  -- pchsource "Mighty/src/mtpch.cpp"

  files
  {
    "%{prj.name}/src/**.h",
    "%{prj.name}/src/**.cpp",
    "%{prj.name}/vendor/glm/glm/**.hpp",
    "%{prj.name}/vendor/glm/glm/**.inl",
    "%{prj.name}/vendor/lua/**.h",
    "%{prj.name}/vendor/lua/**.hpp",
    "%{prj.name}/vendor/sdl/include/**.h"
  }

  defines
  {
    "_CRT_SECURE_NO_WARNINGS"
  }

  includedirs
  {
    "%{prj.name}/src",
    "%{IncludeDir.glm}/glm",
    "%{IncludeDir.lua}",
    "%{IncludeDir.sdl}/include"
  }

  libdirs
  {
    "%{IncludeDir.sdl}/lib/x64/"
  }

  links
  {
    "SDL2.lib",
    "SDL2main.lib",
    "SDL2_image.lib",
    "SDL2_ttf.lib",
    "SDL2_mixer.lib"
  }

  filter "system:windows"
    systemversion "latest" --10.0.17763.0

    -- defines
    -- {
    --   "MT_PLATFORM_WINDOWS",
    --   "MT_BUILD_DLL",
    --   "GLFW_INCLUDE_NONE"
    -- }
    postbuildcommands
    {
      ("{COPY} vendor/sdl/lib/x64/*.dll \"../bin/" .. outputdir .. "/Mighty/\""),
      ("{COPY} assets \"../bin/" .. outputdir .. "/Mighty/\"assets")
    }

  filter "configurations:Debug"
    defines "MT_DEBUG"
    runtime "Debug"
    symbols "on"
  
  filter "configurations:Release"
    defines "MT_RELEASE"
    runtime "Release"
    optimize "on"
  
  filter "configurations:Dist"
    defines "MT_DIST"
    runtime "Release"
    optimize "on"