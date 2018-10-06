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

    map_element_t(const bounding_box_t& bounding_box, Type type) : bounding_box(bounding_box), type(type) {}

    bool is_wall() const {
        return
          this->type == Type::SOLID ||
          this->type == Type::ICE ||
          this->type == Type::SPRING;
    }

    bool is_floor() const {
        return
            this->type == Type::SOLID ||
            this->type == Type::ICE ||
            this->type == Type::SPRING;
    }

    bool is_ceil() const {
        return
          this->type == Type::SOLID ||
          this->type == Type::ICE ||
          this->type == Type::SPRING;
    }

    bool is_ice() const {
        return
            this->type == Type::ICE;
    }

    bool is_spring() const {
        return
            this->type == Type::SPRING;
    }


};


#endif //JUMPNBUMP_MAP_ELEMENT_T_H
