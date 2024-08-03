project "Common"
    kind "StaticLib"
    language "C"
    cdialect "C99"
    staticruntime "off"

    targetdir("../bin/" .. outputdir .. "/%{prj.name}")
    objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/**.c",
        "Source/**.h"
    }

    links {
        "PDCurses"
    }

    includedirs {
        "Source",
        "../Vendor/pdcurses-premake/Source"
    }

    defines {
        "WINDOWS"
    }

    filter { "configurations:Debug" }
        buildoptions "/MTd"
        runtime "Debug"
        symbols "on"
    
    filter { "configurations:Release" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"
        symbols "on"
    
    filter { "configurations:Dist" }
        buildoptions "/MT"
        runtime "Release"
        optimize "on"
        symbols "off"