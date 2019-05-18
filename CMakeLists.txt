cmake_minimum_required(VERSION 3.11)


project(MediaManager)
set(TOY_VERSION_MAJOE 1)
set(TOY_VERSION_MINOR 0)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include_directories(include 
    # third_part/assimp third_part/glad third_part/glfw
    # third_part/glm third_part/stb
)
link_directories(lib)
link_libraries(
    sqlite3)

aux_source_directory(src/cpp SOURCES)
# add_executable(mediaManager ${SOURCES})
add_executable(mediaManager src/main.cpp src/FileSystem.cpp src/Scanner.cpp src/DBConnection.cpp)
if ( MSVC )
    set_target_properties( mediaManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( mediaManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( mediaManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )
endif ( MSVC )