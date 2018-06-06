//
// Created by edwin on 05-06-18.
//

#include "map_position_t.h"
#include "screen_position_t.h"
#include "position_t.h"

map_position_t::map_position_t(const position_t& position) : map_position_t{screen_position_t{position}} {}
map_position_t::map_position_t(const screen_position_t& position) : map_position_t{position.x >> 4, position.y >> 4}{}
