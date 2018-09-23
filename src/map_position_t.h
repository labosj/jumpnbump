//
// Created by edwin on 05-06-18.
//

#ifndef JUMPNBUMP_MAP_POSITION_T_H
#define JUMPNBUMP_MAP_POSITION_T_H

class screen_position_t;
class position_t;

#include "map_position_component_t.h"

class map_position_t {

public:
    map_position_component_t x;
    map_position_component_t y;

    map_position_t() : map_position_t(0, 0) {}

    map_position_t(map_position_component_t x, map_position_component_t y) : x(x), y(y) {}

    map_position_t(const screen_position_t& position);
    map_position_t(const position_t& position);

    map_position_t operator+(const map_position_t &position) const;

    bool operator==(const map_position_t& position) const {
        return x == position.x && y == position.y;
    }
};


#endif //JUMPNBUMP_MAP_POSITION_T_H
