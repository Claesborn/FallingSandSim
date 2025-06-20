---@format disable
add_rules("mode.debug", "mode.release")

add_requires("sfml", "imgui", "imgui-sfml")

set_languages("cxxlatest")

target("Project5")
    set_kind("static")
    add_files("*.cpp")
    add_includedirs("./", { public = true })
    add_packages("sfml", "imgui", "imgui-sfml")

target("FileName")
    set_kind("binary")
    add_files("FileName.cpp")
    add_deps("Project5")

target("Source")
    set_kind("binary")
    add_files("Source.cpp")
    add_deps("Project5")
