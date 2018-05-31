cmake_minimum_required(VERSION 2.8.4)
project(jumpnbump)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_STANDARD 11)
add_definitions(-Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DNDEBUG -DUSE_SDL -DZLIB_SUPPORT -DBZLIB_SUPPORT)

find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)



## This must be set because a bug in ubuntu cmake config file.
## see https://stackoverflow.com/questions/45730098/cmake-leading-or-trailing-whitespace-policy-cmp0004
set(SDL2_LIBRARIES "-L${SDL2_LIBDIR} -lSDL2 -lSDL2_mixer")

include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${BZIP2_INCLUDE_DIRS})

include_directories(${CMAKE_SOURCE_DIR})

add_executable(jumpnbump sdl/gfx.cpp sdl/input.cpp sdl/interrpt.cpp sdl/sound.cpp filter.cpp menu.cpp main.cpp)

target_link_libraries(jumpnbump ${SDL2_LIBRARIES})
target_link_libraries(jumpnbump ${ZLIB_LIBRARIES})
target_link_libraries(jumpnbump ${BZIP2_LIBRARIES})