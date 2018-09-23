//
// Created by edwin on 21-09-18.
//

#include "position_component_t.h"
#include "screen_position_component_t.h"
#include "map_position_component_t.h"

position_component_t::position_component_t(const screen_position_component_t& position_component) : position_component_t{position_component.value << 16} {}
position_component_t::position_component_t(const map_position_component_t& position_component) : position_component_t{screen_position_component_t{position_component}} {}