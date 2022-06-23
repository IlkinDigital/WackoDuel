workspace 'WackoDuel'
    architecture 'x64'
    configurations{'Debug', 'Release', 'Distribution'}

outputdir = '%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'
EnginePath = 'D:/Lester_Files/dev/Projects/VastEngine' 
project 'WackoDuel'
    kind 'SharedLib'
    language 'C++'
    cppdialect 'C++20'
    staticruntime 'off'

    targetdir('Binaries/' ..outputdir .. '/%{prj.name}')
    objdir('Binaries-Int/' ..outputdir .. '/%{prj.name}')

    files {'Source/**.h', 'Source/**.cpp'}

    pchheader "enginepch.h"
    pchsource "Source/enginepch.cpp"

    includedirs
    {
        'Source',
        '%{EnginePath}/Vast/Source',
        '%{EnginePath}/Vast/Source/Vast',
        '%{EnginePath}/Vast/Source/Vast/Core',
        '%{EnginePath}/Vast/Vendor/spdlog/include',
        '%{EnginePath}/Vast/Vendor/entt',
        '%{EnginePath}/Vast/Vendor/glm'
    }

    defines
    {
        '_CRT_SECURE_NO_WARNINGS',
        'VAST_SCRIPT_DLL'
    }

    links
    {
        'Engine/Vast.lib',
        'Engine/GLFW.lib',
        'Engine/ImGui.lib',
        'Engine/Glad.lib'
    }

    filter 'system:windows'
        systemversion 'latest'

    defines
    {
        'VAST_PLATFORM_WINDOWS'
    }

    filter 'configurations:Debug'
        defines 'VAST_CONFIG_DEBUG'
        runtime 'Debug'
        symbols 'on'

    filter 'configurations:Release'
        defines 'VAST_CONFIG_RELEASE'
        runtime 'Release'
        optimize 'on'

    filter 'configurations:Distribution'
        defines 'VAST_CONFIG_DISTRIBUTION'
        runtime 'Release'
        optimize 'on'
