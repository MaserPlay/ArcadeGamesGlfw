cmake_minimum_required(VERSION 3.28)
project(ArcadeGames)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})

add_compile_definitions(APPVERSION="1.3.1")
add_compile_definitions(APPVERSIONCODE=6)
add_compile_definitions(APPNAME="${PROJECT_NAME}")
add_compile_definitions(APPAUTHOR="MaserPlay")


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


list(APPEND cpps src/*.cpp src/Utils/*.cpp src/Games/Snake/*.cpp src/Games/TileEngine/*.cpp ico.rc)
list(APPEND hs src/ src/Games/Snake/ src/Games/TileEngine/ src/Utils/)
if (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    list(APPEND hs src/Games/Pacman/ src/Games/Test)
    list(APPEND cpps src/Games/Pacman/*.cpp src/Games/Test/*.cpp)
endif()


include_directories(${PROJECT_NAME} ${hs})
file(GLOB cpps ${cpps})

add_subdirectory(GameEngine)

if (WIN32)
    target_compile_definitions(Boxer PRIVATE UNICODE)
endif (WIN32)
if (${CMAKE_BUILD_TYPE} STREQUAL "Release" AND WIN32)
    add_compile_definitions(DO_WINMAIN)
    add_executable(${PROJECT_NAME} WIN32 ${cpps})
else ()
    add_executable(${PROJECT_NAME} ${cpps})
endif()

#py
add_custom_target(assets)
add_dependencies(${PROJECT_NAME} assets)
add_custom_command(TARGET assets POST_BUILD
        COMMAND "C:\\Users\\super\\AppData\\Local\\Programs\\Python\\Python312\\python.exe" ${CMAKE_SOURCE_DIR}/archivate_assets.py ${CMAKE_SOURCE_DIR}/Assets ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Generating Assets")

# LIBRARIES

target_link_libraries(${PROJECT_NAME} Engine imgui::imgui)