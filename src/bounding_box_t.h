//
// Created by edwin on 22-09-18.
//

#ifndef JUMPNBUMP_BOUNDING_BOX_T_H
#define JUMPNBUMP_BOUNDING_BOX_T_H

#include "position_t.h"
#include <cstdio>

class bounding_box_t {

public:
    position_t position;

    int width;
    int height;

    bounding_box_t(const position_t& position, int width, int height) : position(position), width(width), height(height) {}

    position_component_t get_left() const { return this->position.x; }

    bounding_box_t get_left_box() const { return bounding_box_t{this->get_top_left(), 1, this->height}; }

    bounding_box_t get_right_box() const { return bounding_box_t{this->get_top_right(), 1, this->height}; }

    bounding_box_t get_top_box() const { return bounding_box_t{this->get_top_left(), this->width, 1}; }

    bounding_box_t get_bottom_box() const { return bounding_box_t{this->get_bottom_left(), this->width, 1}; }

    bounding_box_t get_below_box() const { return bounding_box_t{this->get_bottom_left().below(), this->width, 1}; }

    position_component_t get_right() const { return this->position.x + (this->width - 1); }

    position_component_t get_top() const { return this->position.y; }

    position_component_t get_bottom() const { return this->position.y + (this->height - 1); }

    position_t get_top_left() const { return position_t{this->get_left(), this->get_top()}; }

    position_t get_top_right() const { return position_t{this->get_right(), this->get_top()}; }

    position_t get_bottom_left() const { return position_t{this->get_left(), this->get_bottom()}; }

    bool is_below(const position_component_t& above) const {
        return this->position.y.value - 1 == above.value;
    }

    bool collide(const bounding_box_t& box) const {
        if ( box.get_right() < this->get_left() ) return false;
        if ( this->get_right() < box.get_left() ) return false;
        if ( box.get_bottom() < this->get_top() ) return false;
        if ( this->get_bottom() < box.get_top() ) return false;
        return true;
    }
};


#endif //JUMPNBUMP_BOUNDING_BOX_T_H
