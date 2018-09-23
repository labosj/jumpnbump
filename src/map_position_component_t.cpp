//
// Created by edwin on 21-09-18.
//

#include "map_position_component_t.h"
#include "position_component_t.h"
#include "screen_position_component_t.h"


map_position_component_t::map_position_component_t(const position_component_t& position_component) : map_position_component_t{screen_position_component_t{position_component}} {}
map_position_component_t::map_position_component_t(const screen_position_component_t& position_component) : map_position_component_t{position_component.value >> 4}{}