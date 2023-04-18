---------------------------------
-- [ WORKSPACE CONFIGURATION ] --
---------------------------------
workspace "Universal Microcode Compiler"

	configurations {
		"Debug",
		"Release"
	}

	platforms {
		"Windows",
		"Linux",
		"MacOS"
	}

	--------------------------------
	-- [ PLATFORM CONFIGURATION ] --
	--------------------------------
	filter "platforms:Windows"
		system  "windows"

	filter "platforms:Linux"
		system  "linux"

	filter "platforms:MacOS"
		system  "macosx"

	-- Windows only
	filter { "system:windows", "action:vs*" }
		flags { "MultiProcessorCompile", "NoMinimalRebuild" }
		systemversion "latest"

	-------------------------------------
	-- [ DEBUG/RELEASE CONFIGURATION ] --
	-------------------------------------
	filter "configurations:Debug"
		defines {
			"_DEBUG",
		}
		symbols  "On"
		optimize "Off"
		runtime  "Debug"

	filter "configurations:Release"
		defines {
			"NDEBUG",
		}
		symbols  "Off"
		optimize "Speed"
		runtime  "Release"

	filter {}

	--------------------------------
	-- [ LIBRARIES (vendor/...) ] --
	--------------------------------
	
	include("premake5-vendor.lua")

	-------------------------------
	-- [ PROJECT CONFIGURATION ] --
	-------------------------------

	dir_bin     = "%{wks.location}/_bin/"
	dir_build   = "%{wks.location}/_build/"
	dir_config  = "%{string.lower(cfg.platform)}-%{string.lower(cfg.buildcfg)}/"
	dir_project = "%{string.lower(prj.name)}"

	dir_group = "projects/"
	include("projects/")

	group "Vendor"
		dir_group = "vendor/"
		include("vendor/")
