//
// Created by edwin on 21-09-18.
//

#ifndef JUMPNBUMP_POSITION_COMPONENT_T_H
#define JUMPNBUMP_POSITION_COMPONENT_T_H

class screen_position_component_t;
class map_position_component_t;

class position_component_t {
public:
    int value;

    position_component_t() : position_component_t(0) {}

    position_component_t(int value) : value(value) {}

    position_component_t(const position_component_t& position) : position_component_t{position.value} {}
    position_component_t(const screen_position_component_t& position);
    position_component_t(const map_position_component_t& position);

    position_component_t operator+(const position_component_t& position) const {
        return position_component_t{
                this->value + position.value
        };
    }

    bool operator!=(const position_component_t& position) {
        return this->value != position.value;
    }


    position_component_t operator+=(const position_component_t& position) {
        return this->value += position.value;
    }

    position_component_t operator-=(const position_component_t& position) {
        return this->value -= position.value;
    }

    bool operator<(const position_component_t& position) const {
        return this->value < position.value;
    }

    bool operator>(const position_component_t& position) const {
        return this->value > position.value;
    }
};

#endif //JUMPNBUMP_POSITION_COMPONENT_T_H
