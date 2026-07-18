# Configure from the project root:
# cmake -S . -B backend/build
#
# Build:
# cmake --build backend/build --config Debug
#
# Build and run:
# cmake --build backend/build --config Debug --target run

cmake_minimum_required(VERSION 3.21)

project(MissileDefenseSimulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Backend source files
file(
    GLOB SRC_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/backend/src/*.cpp"
)

file(
    GLOB COMPONENTS_FILES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/backend/src/components/*.cpp"
)

add_compile_definitions(_WIN32_WINNT=0x0A00)

find_package(
    Boost
    CONFIG
    REQUIRED
    COMPONENTS system
)

find_package(
    nlohmann_json
    CONFIG
    REQUIRED
)

add_executable(
    missile_defense_backend
    ${SRC_FILES}
    ${COMPONENTS_FILES}
)

target_include_directories(
    missile_defense_backend
    PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/backend/include"
)

target_link_libraries(
    missile_defense_backend
    PRIVATE
        Boost::system
        nlohmann_json::nlohmann_json
        Shell32
)

# Copy the root frontend directory beside the executable.
add_custom_command(
    TARGET missile_defense_backend
    POST_BUILD
    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/frontend"
        "$<TARGET_FILE_DIR:missile_defense_backend>/frontend"
    COMMENT "Copying frontend files..."
)

# Build and run the application.
add_custom_target(
    run
    COMMAND
        cmd.exe /c start
        "Missile Defense Backend"
        cmd.exe /k
        "\"$<TARGET_FILE:missile_defense_backend>\""
    DEPENDS missile_defense_backend
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:missile_defense_backend>"
    COMMENT "Launching missile defense simulator in a new terminal..."
    USES_TERMINAL
)