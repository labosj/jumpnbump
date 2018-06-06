//
// Created by edwin on 02-06-18.
//

#include "ban_map.h"
#include "util.h"

ban_map_t ban_map;

unsigned int& ban_map_t::get_by_pixel(int x, int y) {
    return this->map[(y) >> 4][(x) >> 4];
}

unsigned int& ban_map_t::get(const map_position_t& pos) {
    return this->map[pos.y][pos.x];
}

const unsigned int& ban_map_t::get(const map_position_t& pos) const {
    return this->map[pos.y][pos.x];
}

unsigned int& ban_map_t::get(int x, int y) {
    return this->map[y][x];
}

const unsigned int& ban_map_t::get_by_pixel(int x, int y) const {
    return this->map[(y) >> 4][(x) >> 4];
}

bool ban_map_t::is_pixel_in_water(int x, int y) const {
    return (this->get_by_pixel((x), ((y) + 7)) == BAN_VOID || this->get_by_pixel(((x) + 15), ((y) + 7)) == BAN_VOID)
           && (this->get_by_pixel((x), ((y) + 8)) == BAN_WATER || this->get_by_pixel(((x) + 15), ((y) + 8)) == BAN_WATER);
}


map_position_t ban_map_t::get_random_position() const {
    return map_position_t{rnd(ban_map_t::WIDTH), rnd(ban_map_t::HEIGHT)};
};

map_position_t ban_map_t::get_random_available_position() const {

    while (true) {
        auto position = get_random_position();

        if (
                this->get(position) == BAN_VOID            //si esta vacio
                ) {
            return position;
        }
    }
}

map_position_t ban_map_t::get_random_available_floor_position() const {

    while (true) {
        auto position = get_random_available_position();
        auto below = position;
        below.y += 1;

        if (
               this->get(below) == BAN_SOLID ||     //y el de abajo es un solido, posiblemente un suelo
               this->get(below) == BAN_ICE  //o un hielo
         )
            return position;
    }
}