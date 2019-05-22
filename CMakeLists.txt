cmake_minimum_required(VERSION 3.11)


project(MediaManager)
set(TOY_VERSION_MAJOE 1)
set(TOY_VERSION_MINOR 0)
set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
cmake_policy(SET CMP0076 NEW)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS)

include_directories(include)
link_directories(lib)
link_libraries(
    sqlite3)

# 1. Cli utils, like mlocate
set(CLI_SOURCE
    cliLocate
    FileSystem
    Scanner
    DBConnection
)
add_executable(clilocate)
foreach(file ${CLI_SOURCE})
    target_sources(clilocate PUBLIC src/${file}.cpp)
endforeach(file ${CLI_SOURCE})

# 2. GUI utils
set(GUI_SOURCE
    LeftView
    MainFrm
    ManagerGUI
    ManagerGUIDoc
    ManagerGUIView
    stdafx
)
add_executable(guiManager WIN32)
foreach(file ${GUI_SOURCE})
    target_sources(guiManager PUBLIC src/MFCFrontEnd/${file}.cpp)
    target_sources(guiManager PUBLIC src/MFCFrontEnd/${file}.h)
endforeach(file ${GUI_SOURCE})
target_sources(guiManager PUBLIC src/MFCFrontEnd/ManagerGUI.rc)
target_sources(guiManager PUBLIC src/MFCFrontEnd/res/ManagerGUI.rc2)

set(CMAKE_MFC_FLAG 1)
target_compile_definitions(guiManager PUBLIC -D_AFXDLL -D_UNICODE -DUNICODE -D_WINDOWS -D_DEBUG)
target_link_options(guiManager PUBLIC "-subsystem:windows")




if ( MSVC )
    set_target_properties( clilocate PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( clilocate PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( clilocate PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )

    set_target_properties( guiManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( guiManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( guiManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )
endif ( MSVC )

