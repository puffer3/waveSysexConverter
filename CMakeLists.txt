cmake_minimum_required(VERSION 3.25)
project(waldorf_patch_converter)

#set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.14.0\\5.14.0\\mingw73_32\\")
#
#find_package(Qt5Widgets REQUIRED)
#
#target_link_libraries(menuGUI Qt5::Widgets)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)

add_executable(
        waldorf_patch_converter
        hexTest.cpp
            )

configure_file(${CMAKE_SOURCE_DIR}/DavidSoundBankA.syx ${CMAKE_BINARY_DIR}/DavidSoundBankA.syx COPYONLY)








