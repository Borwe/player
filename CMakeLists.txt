cmake_minimum_required(VERSION 3.5)

#Replace PROJECT_NAME with project name, and _SOURCES, with sources
set(PROJECT_NAME player)
project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} "src/main.cpp" "src/mainwindow.cpp" 
    "src/vlcplayer.cpp")

#add local include files
target_include_directories(player PUBLIC ./include)

#get location of where CMAKE dirs are located
if(NOT CMAKE_DIR)
    set(CMAKE_DIR ~/vim-setup/vimrc/cmake-toolchain)
endif()

#add PDCURSES
message("CMAKE_DIR: " ${CMAKE_DIR})
include(${CMAKE_DIR}/PDCurses.cmake)
usePDCurses(${PROJECT_NAME})

#add vlcpp for the vlc player
include(${CMAKE_DIR}/VLCPP.cmake)
useVLCPP(${PROJECT_NAME})

#ncurses
#target_link_libraries(player -lncurses -ltinfo)

#add boost libs
include(${CMAKE_DIR}/BOOST.cmake)
useBoostFileSystemStatic(${PROJECT_NAME})

#for easy finishing with static libraries
include(${CMAKE_DIR}/COMPILEMVC.cmake)
compileStatic(${PROJECT_NAME})

#This is for the YCM settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json)
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
ENDIF()
