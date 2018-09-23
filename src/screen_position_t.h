//
// Created by edwin on 05-06-18.
//

#ifndef JUMPNBUMP_SCREEN_POSITION_T_H
#define JUMPNBUMP_SCREEN_POSITION_T_H

class position_t;
class map_position_t;

#include "screen_position_component_t.h"

class screen_position_t {

public:
    screen_position_component_t x;
    screen_position_component_t y;

    screen_position_t() : screen_position_t(0, 0) {}

    screen_position_t(screen_position_component_t x, screen_position_component_t y) : x(x), y(y) {}

    screen_position_t(const position_t& position);
    screen_position_t(const map_position_t& position);

    screen_position_t operator+(const screen_position_t &position) const {
        return screen_position_t{
                this->x + position.x,
                this->y + position.y
        };
    }
};

#endif //JUMPNBUMP_SCREEN_POSITION_T_H
