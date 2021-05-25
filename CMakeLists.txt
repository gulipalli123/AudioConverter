# CMakeList.txt : CMake project for WavToMp3, include source and define
# project specific logic here.
#

#target_link_libraries(WavToMp3  private liblame)
# TODO: Add tests and install targets if needed.

cmake_minimum_required(VERSION 3.11)

# Target Project
project (WavToMp3)

# Begin: Including Sources and Headers
include_directories(include)

file (GLOB SOURCES "src/*.cpp")


if (UNIX)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(THREADS_PREFER_PTHREAD_FLAG ON)
endif (UNIX)

set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
set(CMAKE_EXE_LINKER_FLAGS "-flto")
set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O2 --std=c++17 -Wall -flto" )

enable_language(CXX)

add_executable (WavToMp3 ${SOURCES})
set_target_properties(WavToMp3 PROPERTIES  CXX_STANDARD 17  CXX_STANDARD_REQUIRED ON)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

if (MSVC)
find_package(mp3lame CONFIG REQUIRED)
find_path(DIRENT_INCLUDE_DIRS "dirent.h")
target_include_directories(WavToMp3 PRIVATE ${DIRENT_INCLUDE_DIRS})
target_link_libraries(WavToMp3 PRIVATE mp3lame::mp3lame)
endif (MSVC)

if (UNIX)
find_package(mp3lame REQUIRED)
find_package(Threads REQUIRED)
find_path(MP3LAME_INCLUDE_DIR lame/lame.h)
include_directories(${MP3LAME_INCLUDE_DIR} ${PTHREAD_INCLUDE_DIRS})
set(LIBS ${LIBS} ${MP3LAME_LIBRARIES} Threads::Threads)
target_link_libraries (WavToMp3 ${LIBS} )
endif (UNIX)
