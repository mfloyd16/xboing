local raylib = require "build_files.raylib"

workspace "xboing"
    configurations { "Debug", "Release" }

    filter "configurations:Debug"
        symbols "On"
    filter "configurations:Release"
        optimize "On"
    filter {}

    startproject "rayboing"

    project "rayboing"
        kind "ConsoleApp"
        language "C"
        location "build_files"
        targetdir "bin/%{cfg.buildcfg}"

        raylib.setup_project()

        includedirs { "include", "src" }

        vpaths {
            ["Header Files/*"] = { "include/**.h" },
            ["Source Files/*"] = { "src/**.c" }
        }

        files { "src/**.c", "include/**.h"}

    project "raylib"
        raylib.static_lib_target()


-- Clean action: remove common build/output directories
newoption {
    trigger = "all",
    description = "With 'clean': also remove generated project files (USE WITH CAUTION)"
}

newaction {
    trigger     = "clean",
    description = "Remove generated build outputs (bin, obj, build, lib, .vs). Use --all to remove generated project files.",
    execute     = function ()
        local function info(msg) print("[clean] " .. msg) end

        -- List of directories to remove by default
        local targets = {
            "bin",
            "obj",
            "build",
            "lib",
            ".vs"
        }

        -- Remove directories that exist
        for _, dir in ipairs(targets) do
            if os.isdir(dir) then
                info("Removing directory: " .. dir)
                os.rmdir(dir)
            else
                info("Not present: " .. dir)
            end
        end

        -- Remove some common build_files subdirs (if present)
        local extraDirs = {
            "build_files/obj",
            "build_files/Debug",
            "build_files/Release",
        }
        for _, d in ipairs(extraDirs) do
            if os.isdir(d) then
                info("Removing directory: " .. d)
                os.rmdir(d)
            end
        end

        -- Optionally remove generated project files (sln, vcxproj, etc.)
        if _OPTIONS["all"] then
            info("Removing generated project files (build_files/*.vcxproj*, **/*.vcxproj.filters, **/*.vcxproj.user) ...")

            local projFiles = os.matchfiles("build_files/*.vcxproj*")
            for _, f in ipairs(projFiles) do
                info("Removing: " .. f)
                os.remove(f)
            end

            local genFiles = os.matchfiles("**/*.vcxproj.filters")
            for _, f in ipairs(genFiles) do
                info("Removing: " .. f)
                os.remove(f)
            end
            genFiles = os.matchfiles("**/*.vcxproj.user")
            for _, f in ipairs(genFiles) do
                info("Removing: " .. f)
                os.remove(f)
            end
        else
            info("Run 'premake5 clean --all' to remove generated project files (use with caution).")
        end

        info("Clean complete.")
    end
}
