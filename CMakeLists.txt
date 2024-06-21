cmake_minimum_required(VERSION 3.28)
project(ArcadeGamesGLFW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

add_compile_definitions(APPVERSION=0.1)
add_compile_definitions(APPNAME="${PROJECT_NAME}")
add_compile_definitions(APPAUTHOR="MaserPlay")

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)

include_directories(${PROJECT_NAME} src/ src/Games/Snake/ src/Games/Base/ src/Utils/)

file(GLOB cpps src/*.cpp src/Utils/*.cpp src/Games/Snake/*.cpp src/Games/Base/*.cpp)

add_subdirectory(third_party/Boxer)

if (WIN32)
    target_compile_definitions(Boxer PRIVATE UNICODE)
endif (WIN32)

add_executable(${PROJECT_NAME} ${cpps})

if (${CMAKE_BUILD_TYPE} STREQUAL "Release" AND WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)
endif()

target_link_libraries(${PROJECT_NAME} glfw glm::glm GLEW::GLEW Boxer imgui::imgui)