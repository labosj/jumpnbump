//
// Created by edwin on 30-09-18.
//

#ifndef JUMPNBUMP_MAP_ELEMENTS_T_H
#define JUMPNBUMP_MAP_ELEMENTS_T_H

#include <vector>
#include "map_element_t.h"

class map_elements_t {
public:
    std::vector<map_element_t> elements;

    bool empty() const;
    map_elements_t collide(const bounding_box_t& bounding_box) const;
    map_elements_t just_below(const position_component_t& y) const;
    bool is_wall() const;
    bool is_ceil() const;
    bool is_floor() const;
    bool is_slippery() const;
    bool is_spring() const;
    const map_element_t& get_leftmost_wall() const;
    const map_element_t& get_rightmost_wall() const;
    const map_element_t& get_lowest_ceil() const;
    const map_element_t& get_highest_floor() const;

};


#endif //JUMPNBUMP_MAP_ELEMENTS_T_H
