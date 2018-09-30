//
// Created by edwin on 04-06-18.
//

#ifndef JUMPNBUMP_POSITION_T_H
#define JUMPNBUMP_POSITION_T_H

#include "screen_position_t.h"
#include "position_component_t.h"
class map_position_t;

class position_t {
public:
    position_component_t x;
    position_component_t y;

    position_t() : position_t(0,0) {}

    position_t(position_component_t x, position_component_t y) : x(x), y(y) {}

    position_t(const position_t& position) : position_t{position.x, position.y} {}

    position_t(const screen_position_t& position);
    position_t(const map_position_t& position);

    position_t operator+(const position_t& position) const {
        return position_t{
            this->x + position.x,
            this->y + position.y
        };
    }

    position_t below() const {
        return position_t{
                this->x,
                this->y + 1
        };
    }

    position_t right() const {
        return position_t{
            this->x + 1,
            this->y
        };
    }

};


#endif //JUMPNBUMP_POSITION_T_H
