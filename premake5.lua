workspace "ChatWin"
    architecture "x64"

    configurations {
        "Debug",
        "Release"
    }

    startproject "Server"

outputdir = "%{cfg.buildcfg}"

include "Server"
include "Client"
include "Common"