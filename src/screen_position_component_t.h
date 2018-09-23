//
// Created by edwin on 21-09-18.
//

#ifndef JUMPNBUMP_SCREEN_POSITION_COMPONENT_T_H
#define JUMPNBUMP_SCREEN_POSITION_COMPONENT_T_H

class position_component_t;
class map_position_component_t;

class screen_position_component_t {
public:
    int value;

    screen_position_component_t() : screen_position_component_t(0) {}

    screen_position_component_t(int value) : value(value) {}

    screen_position_component_t(const position_component_t& position_component);
    screen_position_component_t(const map_position_component_t& position_component);

    screen_position_component_t operator+(const screen_position_component_t& position) const {
        return screen_position_component_t{
                this->value + position.value
        };
    }
};


#endif //JUMPNBUMP_SCREEN_POSITION_COMPONENT_T_H
