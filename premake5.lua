workspace "ChatWin"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    startproject "Server"

outputdir = "%{cfg.buildcfg}"

os.execute("git submodule update --")

group "Dependencies"
    include "Vendor/pdcurses-premake"
group ""

include "Server"
include "Client"
include "Common"