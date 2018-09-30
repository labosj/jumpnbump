//
// Created by edwin on 30-09-18.
//

#ifndef JUMPNBUMP_MAP_ELEMENT_T_H
#define JUMPNBUMP_MAP_ELEMENT_T_H


#include "bounding_box_t.h"

class map_element_t {
public:
    enum class Type {
        VOID = 0,
        SOLID = 1,
        WATER = 2,
        ICE = 3,
        SPRING = 4
    };

    bounding_box_t bounding_box;
    Type type;


};


#endif //JUMPNBUMP_MAP_ELEMENT_T_H
