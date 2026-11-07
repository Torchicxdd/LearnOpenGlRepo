cmake_minimum_required(VERSION 3.15)
project(GraphicsLearning CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
	add_compile_options(
		-Wall
		-Weffc++
		-Wextra
		-Wconversion
		-Wsign-conversion
		-pedantic-errors
	)
endif()

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

add_executable(GraphicsLearning ${PROJECT_SOURCES})

target_compile_definitions(GraphicsLearning PRIVATE GLFW_INCLUDE_NONE)

target_include_directories(GraphicsLearning PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

target_link_libraries(GraphicsLearning PRIVATE 
    glfw 
    d3d11
    dxgi 
	d3dcompiler
	glad::glad
)

# Copy all assets to build directory post build
add_custom_command(TARGET GraphicsLearning POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${CMAKE_SOURCE_DIR}/assets"
	"$<TARGET_FILE_DIR:GraphicsLearning>/assets"
)