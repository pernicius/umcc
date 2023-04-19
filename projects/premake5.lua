-----------------------
-- [ PROJECT CONFIG] --
-----------------------
project "umcc"
	architecture  "x86_64"
	language      "C++"
	cppdialect    "C++20"
	staticruntime "On"

	targetdir (dir_bin   .. dir_group .. dir_config)
	objdir    (dir_build .. dir_group .. dir_config .. dir_project)

	pchheader "pch.h"
	pchsource "source/pch.cpp"

	-- Libraries
	VendorCxxopts{}
	VendorColoredCout{}


	includedirs {
		"source",
	}


	files {
		"**.h",
		"**.c",
		"**.hpp",
		"**.cpp"
	}


	filter "configurations:Debug"

		kind "ConsoleApp"

		defines {
		}


	filter "configurations:Release"

		kind "ConsoleApp"
--		kind "WindowedApp"

		defines {
		}


	filter {}


--	prebuildmessage "Updating version information..."
--	prebuildcommands {
--		"\"%{wks.location}Tools/build_inc/_bin/build_inc_" .. os.host() .. "\" -bfile \"%{prj.location}Source/Config/Version.h\" -bdef VERSION_BUILD"
--	}
--
--
--	postbuildmessage "Copying assets to the target folder..."
--	postbuildcommands {
--		"{COPYDIR} %{wks.location}Source/Client/GameEngine/Assets " .. dir_bin .. dir_group .. dir_config .. "Assets",
--		"{COPYDIR} %{prj.location}Assets " .. dir_bin .. dir_group .. dir_config .. "Assets"
--	}
