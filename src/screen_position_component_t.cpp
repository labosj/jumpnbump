//
// Created by edwin on 21-09-18.
//

#include "screen_position_component_t.h"
#include "position_component_t.h"
#include "map_position_component_t.h"

screen_position_component_t::screen_position_component_t(const position_component_t& position_component) : screen_position_component_t{position_component.value >> 16}{}
screen_position_component_t::screen_position_component_t(const map_position_component_t& position_component) : screen_position_component_t{position_component.value << 4}{}