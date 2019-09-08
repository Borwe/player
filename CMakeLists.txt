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
include(${CMAKE_DIR}/PDCurses.cmake)
target_include_directories(player PUBLIC ${PDCURSES_INCLUDES_DIR})
target_link_directories(player PUBLIC  ${PDCURSES_LINKS_DIR})
target_link_libraries(player ${PDCURSES_LINKS})

#add vlcpp for the vlc player
include(${CMAKE_DIR}/VLCPP.cmake)
target_include_directories(player PUBLIC ${VLCPP_INCLUDES})
target_link_libraries(player ${VLCPP_LINKS})

#ncurses
#target_link_libraries(player -lncurses -ltinfo)

#add boost libs
target_link_libraries(player boost_system.a boost_filesystem.a)


#This is for the YCM settings
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json)
  EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy_if_different  ${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json)
ENDIF()
