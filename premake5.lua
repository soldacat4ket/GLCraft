workspace "OpenGLMinecraftClone"
	configurations { "Debug", "Release" }
	platforms { "x86_64" }

project "OpenGLMinecraftClone"
	language "C++"
	cppdialect "C++23"
	targetdir "bin/%{cfg.buildcfg}-%{cfg.platform}"
	objdir "bin-int/%{cfg.buildcfg}-%{cfg.platform}"

	files { 
		"src/**.h", 
		"src/**.cpp", 
		"res/**.json", 
		"res/**.glsl", 
		"vendor/glad/src/glad.c" 
	}
	
	-- 64 bit version of GLFW, be sure to replace all libs to their correct version
	-- will eventaully be updated to compile as a project in our solution
	libdirs { "vendor/glfw-3.4.bin.WIN64/lib-vc2022" }
	
	links { "glfw3dll" }
	
	includedirs { 
		"src", 
		"vendor/glfw-3.4.bin.WIN64/include",
		"vendor/glad/include", 
		"vendor/glm",
		"vendor/stb_image",
		"vendor/nlohmann-json/include",
		"vendor/spdlog-1.x/include"
	}

	postbuildcommands {
      "{COPYFILE} %{path.getabsolute('vendor/glfw-3.4.bin.WIN64/lib-vc2022/glfw3.dll')} %{cfg.targetdir}",
	  "{COPYDIR} %{path.getabsolute('res')} %{cfg.targetdir}/res"
	}

	filter { "configurations:Debug" }
		kind  "ConsoleApp"
		defines { "_DEBUG" }
		symbols "On"
	
	filter "configurations:Release"
		kind "WindowedApp"
		defines { "NDEBUG" }
		optimize "On"
