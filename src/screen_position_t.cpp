//
// Created by edwin on 05-06-18.
//

#include "screen_position_t.h"

#include "position_t.h"
#include "map_position_t.h"

screen_position_t::screen_position_t(const position_t& position) : screen_position_t{position.x, position.y}{}
screen_position_t::screen_position_t(const map_position_t& position) : screen_position_t{position.x, position.y}{}