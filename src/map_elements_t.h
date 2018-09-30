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

    map_elements_t collide(const bounding_box_t& bounding_box) const;
    bool is_wall() const;
    bool is_ceil() const;
    const map_element_t& get_leftmost_wall() const;
    const map_element_t& get_rightmost_wall() const;
    const map_element_t& get_lowest_ceil() const;
};


#endif //JUMPNBUMP_MAP_ELEMENTS_T_H
