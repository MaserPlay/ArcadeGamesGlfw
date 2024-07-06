cmake_minimum_required(VERSION 3.28)
project(ArcadeGames)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

add_compile_definitions(APPVERSION=1.1)
add_compile_definitions(APPVERSIONCODE=2)
add_compile_definitions(APPNAME="${PROJECT_NAME}")
add_compile_definitions(APPAUTHOR="MaserPlay")

#LOCAL DEFINE
if (SNAKE_ANIMATION)
    add_compile_definitions(SNAKE_ANIMATION)
endif (SNAKE_ANIMATION)



find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(libzip CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

include_directories(${PROJECT_NAME} src/ src/Games/Snake/ src/Games/TileEngine/ src/Games/Base/#[[ src/Games/Pacman/]] src/Utils/)

file(GLOB cpps src/*.cpp src/Utils/*.cpp src/Games/Snake/*.cpp src/Games/TileEngine/*.cpp#[[ src/Games/Pacman/*.cpp]] src/Games/Base/*.cpp)

add_subdirectory(third_party/Boxer)
add_subdirectory(third_party/nativefiledialog-extended)

if (WIN32)
    target_compile_definitions(Boxer PRIVATE UNICODE)
endif (WIN32)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release" AND WIN32)
    add_compile_definitions(DO_WINMAIN)
    add_executable(${PROJECT_NAME} WIN32 ${cpps} ico.rc)
else ()
    add_executable(${PROJECT_NAME} ${cpps} ico.rc)
endif()

target_link_libraries(${PROJECT_NAME} cpr::cpr Freetype::Freetype nlohmann_json::nlohmann_json SndFile::sndfile glfw glm::glm GLEW::GLEW Boxer spdlog::spdlog nfd imgui::imgui OpenAL::OpenAL ${Stb_INCLUDE_DIR} libzip::zip)