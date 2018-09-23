//
// Created by edwin on 21-09-18.
//

#ifndef JUMPNBUMP_MAP_POSITION_COMPONENT_T_H
#define JUMPNBUMP_MAP_POSITION_COMPONENT_T_H

class position_component_t;
class screen_position_component_t;

class map_position_component_t {
public:
    int value;

    map_position_component_t() : map_position_component_t(0) {}

    map_position_component_t(int value) : value(value) {}

    map_position_component_t(const position_component_t& position);
    map_position_component_t(const screen_position_component_t& position);

    map_position_component_t operator+(const map_position_component_t& position) const {
        return map_position_component_t{
                this->value + position.value
        };
    }

    bool operator==(const map_position_component_t& position) const {
        return value == position.value;
    }
};


#endif //JUMPNBUMP_MAP_POSITION_COMPONENT_T_H
