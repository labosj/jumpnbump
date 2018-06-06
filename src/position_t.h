//
// Created by edwin on 04-06-18.
//

#ifndef JUMPNBUMP_POSITION_T_H
#define JUMPNBUMP_POSITION_T_H

#include "screen_position_t.h"

class position_t {
public:
    int x;
    int y;

    position_t() : position_t(0,0) {}

    position_t(int x, int y) : x(x), y(y) {}

    screen_position_t to_pixels() const {
        return screen_position_t{*this};
    }

    position_t operator+(const position_t& position) {
        return position_t{
            this->x + position.x,
            this->y + position.y
        };
    }

};


#endif //JUMPNBUMP_POSITION_T_H
