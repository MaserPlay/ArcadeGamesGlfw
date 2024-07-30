cmake_minimum_required(VERSION 3.28)
project(Engine)

set(CMAKE_CXX_STANDARD 17)

add_compile_definitions(ENGINEVERSION="0.1b")
add_compile_definitions(ENGINEVERSIONCODE=0)
add_compile_definitions(ENGINENAME="${PROJECT_NAME}")
add_compile_definitions(ENGINEAUTHOR="MaserPlay")

list(APPEND cpps src/System/*.cpp src/Audio/*.cpp src/Render/*.cpp src/Render/UI/*.cpp)
list(APPEND dirs src/ src/System/ src/Audio/ src/Render/ src/Render/UI/)

add_subdirectory(third_party/Boxer)
add_subdirectory(third_party/nativefiledialog-extended)

if (WIN32)
    target_compile_definitions(Boxer PRIVATE UNICODE)
endif (WIN32)

file(GLOB cpps ${cpps})

add_library(${PROJECT_NAME} STATIC ${cpps})

target_include_directories(${PROJECT_NAME} PUBLIC ${dirs})

include_directories(${dirs})

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenAL CONFIG REQUIRED)
find_package(SndFile CONFIG REQUIRED)
find_package(Stb REQUIRED)
find_package(libzip CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(Freetype REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# LIBRARIES

target_link_libraries(${PROJECT_NAME} PUBLIC cpr::cpr Freetype::Freetype nlohmann_json::nlohmann_json SndFile::sndfile glfw glm::glm GLEW::GLEW Boxer spdlog::spdlog nfd OpenAL::OpenAL ${Stb_INCLUDE_DIR} libzip::zip)