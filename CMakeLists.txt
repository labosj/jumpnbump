cmake_minimum_required(VERSION 3.10)
project(jumpnbump)

find_package(SDL2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(BZip2 REQUIRED)

## This must be set because a bug in ubuntu cmake config file.
## see https://stackoverflow.com/questions/45730098/cmake-leading-or-trailing-whitespace-policy-cmp0004
set(SDL2_LIBRARIES "-L${SDL2_LIBDIR} -lSDL2 -lSDL2_mixer")

add_executable(jumpnbump
        src/sdl/gfx.cpp
        src/sdl/input.cpp
        src/sdl/interrpt.cpp
        src/sdl/sound.cpp

        src/object_t.cpp
        src/util.cpp
        src/player.cpp
        src/ban_map.cpp
        src/draw.cpp
        src/data.cpp
        src/main_info.cpp
        src/gob.cpp
        src/menu.cpp
        src/main.cpp
        src/menu.h
        src/object_anim.h src/sdl/gfx.h src/pob_t.h src/position_t.cpp src/position_t.h src/screen_position_t.cpp src/screen_position_t.h src/map_position_t.cpp src/map_position_t.h)

target_compile_features(jumpnbump PRIVATE cxx_std_14)
target_compile_definitions(jumpnbump PRIVATE -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DNDEBUG -DUSE_SDL -DZLIB_SUPPORT -DBZLIB_SUPPORT)

target_include_directories(jumpnbump PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(jumpnbump PRIVATE ${ZLIB_INCLUDE_DIRS})
target_include_directories(jumpnbump PRIVATE ${BZIP2_INCLUDE_DIRS})
target_include_directories(jumpnbump PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(jumpnbump PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(jumpnbump PRIVATE ${ZLIB_LIBRARIES})
target_link_libraries(jumpnbump PRIVATE ${BZIP2_LIBRARIES})