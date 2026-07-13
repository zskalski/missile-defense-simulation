cmake_minimum_required(VERSION 3.21)

project(MissileDefenseBackend LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_definitions(_WIN32_WINNT=0x0A00)

find_package(Boost CONFIG REQUIRED COMPONENTS system)
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(
    missile_defense_backend
    src/main.cpp
    src/WebSocketServer.cpp
    src/MissileDefenseSimulator.cpp
)

target_include_directories(
    missile_defense_backend
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(
    missile_defense_backend
    PRIVATE
        Boost::system
        nlohmann_json::nlohmann_json
)