project "Server"
    kind "ConsoleApp"
    language "C"
    cdialect "C99"
    staticruntime "on"

    targetdir("../bin/" .. outputdir .. "/%{prj.name}")
    objdir("../bin-int/" .. outputdir .. "/%{prj.name}")

    files {
        "Source/**.c",
        "Source/**.h"
    }

    links {
        "Common",
        "PDCurses",
        "llib"
    }

    includedirs {
        "../Common/Source",
        "../Vendor/pdcurses-premake/Source",
        "../Vendor/llib/Source"
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