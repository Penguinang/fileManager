cmake_minimum_required(VERSION 3.11)
project(MediaManager)

set(TOY_VERSION_MAJOE 1)
set(TOY_VERSION_MINOR 0)
set(CMAKE_BUILD_TYPE "Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
cmake_policy(SET CMP0076 NEW)

add_compile_definitions(_CRT_SECURE_NO_WARNINGS
#  _UNICODE UNICODE
 )

include_directories(include src/MFCFrontEnd)
link_directories(lib)
link_libraries(sqlite3)
add_compile_options(/MTd)
set(CMAKE_MFC_FLAG 1)

#################################################################################################################
# 1. Cli utils, like mlocate
#################################################################################################################
set(CLI_SOURCE
    FileSystem
    Scanner
    DBConnection
)
add_executable(clilocate)
foreach(file ${CLI_SOURCE})
    target_sources(clilocate PUBLIC src/${file}.cpp)
    target_sources(clilocate PUBLIC include/${file}.h)
endforeach(file ${CLI_SOURCE})
target_sources(clilocate PUBLIC src/cliLocate.cpp)
target_sources(clilocate PUBLIC include/tstring.h)
target_sources(clilocate PUBLIC src/MFCFrontEnd/stdafx.h)
target_sources(clilocate PUBLIC src/MFCFrontEnd/stdafx.cpp)


#################################################################################################################
# 2. GUI utils
#################################################################################################################
set(GUI_SOURCE
    LeftView
    CRightView
    MainFrm
    ManagerGUI
    ManagerGUIDoc
    ManagerGUIView
    UpdateDialog
    stdafx
)
add_executable(guiManager WIN32)
foreach(file ${GUI_SOURCE})
    target_sources(guiManager PUBLIC src/MFCFrontEnd/${file}.cpp)
    target_sources(guiManager PUBLIC src/MFCFrontEnd/${file}.h)
endforeach(file ${GUI_SOURCE})
foreach(file ${CLI_SOURCE})
    target_sources(guiManager PUBLIC src/${file}.cpp)
    target_sources(guiManager PUBLIC include/${file}.h)
endforeach(file ${CLI_SOURCE})

target_sources(guiManager PUBLIC include/tstring.h)
target_sources(guiManager PUBLIC src/MFCFrontEnd/ManagerGUI.rc)
target_sources(guiManager PUBLIC src/MFCFrontEnd/res/ManagerGUI.rc2)
target_include_directories(guiManager PUBLIC src/MFCFrontEnd)

target_compile_definitions(guiManager PUBLIC -D_WINDOWS -D_DEBUG)
target_link_options(guiManager PUBLIC "-subsystem:windows"  )

#################################################################################################################
# Ended
#################################################################################################################

set_target_properties(clilocate guiManager PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")


if ( MSVC )
    set_target_properties( clilocate PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( clilocate PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( clilocate PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )

    set_target_properties( guiManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( guiManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin )
    set_target_properties( guiManager PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin )
endif ( MSVC )

