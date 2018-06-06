//
// Created by edwin on 05-06-18.
//

#ifndef JUMPNBUMP_MAP_POSITION_T_H
#define JUMPNBUMP_MAP_POSITION_T_H

class screen_position_t;
class position_t;

class map_position_t {
public:
    int x;
    int y;

    map_position_t() : map_position_t(0, 0) {}

    map_position_t(int x, int y) : x(x), y(y) {}

    map_position_t(const screen_position_t& position);
    map_position_t(const position_t& position);

    map_position_t operator+(const map_position_t &position) const {
        return map_position_t{
                this->x + position.x,
                this->y + position.y
        };
    }
};


#endif //JUMPNBUMP_MAP_POSITION_T_H
