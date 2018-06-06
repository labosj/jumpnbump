//
// Created by edwin on 04-06-18.
//

#include "position_t.h"

position_t::position_t(const screen_position_t& position) : position_t{position.x << 16, position.y << 16} {}
position_t::position_t(const map_position_t& position) : position_t{screen_position_t{position}} {}