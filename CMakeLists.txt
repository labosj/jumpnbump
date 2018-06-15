cmake_minimum_required(VERSION 3.10)
project(jumpnbump)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
else()
    message(WARNING "The file conanbuildinfo.cmake doesn't exist, you have to run conan install first")
endif()

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

find_package(SFML 2.4 COMPONENTS system window graphics audio)
find_package(SDL2 REQUIRED)

## This must be set because a bug in ubuntu cmake config file.
## see https://stackoverflow.com/questions/45730098/cmake-leading-or-trailing-whitespace-policy-cmp0004
set(SDL2_LIBRARIES "-L${SDL2_LIBDIR} -lSDL2")

add_executable(jumpnbump
        src/sdl/input.cpp
        src/sdl/interrpt.cpp

        src/object_t.cpp
        src/util.cpp
        src/player_t.cpp
        src/ban_map.cpp
        src/data.cpp
        src/main_info.cpp
        src/gob_t.cpp
        src/main.cpp
        src/leftovers_t.cpp
        src/anim_t.h src/pob_t.h src/position_t.cpp src/position_t.h src/screen_position_t.cpp src/screen_position_t.h src/map_position_t.cpp src/map_position_t.h src/pob_t.cpp src/anim_frame_t.cpp src/anim_frame_t.h src/anim_t.cpp src/anim_handler_t.cpp src/anim_handler_t.h src/pobs_t.cpp src/pobs_t.h src/objects_t.cpp src/objects_t.h src/stage_t.cpp src/stage_t.h src/sound_manager_t.cpp src/sound_manager_t.h src/game_manager_t.cpp src/game_manager_t.h)

target_compile_features(jumpnbump PRIVATE cxx_std_14)
target_compile_definitions(jumpnbump PRIVATE -Dstricmp=strcasecmp -Dstrnicmp=strncasecmp -DNDEBUG -DUSE_SDL)

target_include_directories(jumpnbump PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(jumpnbump PRIVATE ${CMAKE_SOURCE_DIR})

target_link_libraries(jumpnbump PRIVATE ${SDL2_LIBRARIES})
target_link_libraries(jumpnbump PRIVATE ${SFML_LIBRARIES})