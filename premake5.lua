workspace "ChatWin"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    startproject "Server"

outputdir = "%{cfg.buildcfg}"

os.execute("git submodule update --init")

group "Dependencies"
    include "./Vendor/pdcurses-premake/premake5.lua"
group ""

include "Server"
include "Client"
include "Common"