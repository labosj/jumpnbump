//
// Created by edwin on 05-06-18.
//

#include "screen_position_t.h"

#include "position_t.h"
#include "map_position_t.h"

screen_position_t::screen_position_t(const position_t& position) : screen_position_t{position.x >> 16, position.y >> 16}{}
screen_position_t::screen_position_t(const map_position_t& position) : screen_position_t{position.x << 4, position.y << 4}{}