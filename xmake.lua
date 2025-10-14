
set_languages("c++20")

if is_plat("windows") then
  set_toolchains("msvc")
  add_cxflags("/std:c++20", "/wd4267", { force = true })
  add_links("opengl32") -- Link against OpenGL on Windows
else
  set_toolchains("clang")
  set_toolset("cc", "clang")
  set_toolset("cxx", "clang")
  add_cxflags("-std=c++20", { force = true })
  add_links("GL")
end

set_warnings("all", "error")
add_rules("mode.debug", "mode.release")

set_optimize("fastest")

add_includedirs("include")

-- add libraries
local project_libs = { "cxxopts", "fmt", "opengl", "libsdl" }

add_requires(table.unpack(project_libs))

target("imgui")
  set_kind("static")
  add_files("include/imgui/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("imgui-sdl2")
  set_kind("static")
  add_deps("imgui")
  add_files("include/imgui-sdl2/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

  target("core")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("entities")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("math")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("models")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("rendering")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

target("scene")
  set_kind("static")
  add_files("src/**/*.cpp")
  add_packages(table.unpack(project_libs))
  set_targetdir("./app")

-- main project executable
target("app")
  set_kind("binary")
  add_files("src/main.cpp")
  add_packages(table.unpack(project_libs))
  add_deps("imgui")
  add_deps("imgui-sdl2")
  add_deps("core")
  add_deps("entities")
  add_deps("math")
  add_deps("models")
  add_deps("rendering")
  add_deps("scene")
  set_targetdir("./app")