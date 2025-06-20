---@format disable
add_rules("mode.debug", "mode.release")

add_requires("sfml", "imgui", "imgui-sfml")

set_languages("cxxlatest")

target("library")
    set_kind("static")
    add_files("src/library/*.cpp")
    add_includedirs("include", { public = true })
    add_packages("sfml", "imgui", "imgui-sfml", { public = true })

target("FileName")
    set_kind("binary")
    add_files("src/file-name/FileName.cpp")
    add_deps("library")
    add_installfiles("assets/**/*")

target("Source")
    set_kind("binary")
    add_files("src/source/Source.cpp")
    add_deps("library")
    add_installfiles("assets/**/*")
