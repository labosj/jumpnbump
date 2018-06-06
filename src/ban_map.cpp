//
// Created by edwin on 02-06-18.
//

#include <fstream>
#include <utility>
#include <algorithm>
#include <iostream>
#include "ban_map.h"
#include "screen_position_t.h"
#include "util.h"

ban_map_t ban_map;


unsigned int ban_map_t::get_by_pixel(int x, int y) const {
    return this->get(screen_position_t{x, y});
}

unsigned int ban_map_t::get(const map_position_t& pos) const {
    //std::cout << "const[" << pos.x << "," << pos.y << "]\n";
    if ( pos.x < 0) return BAN_VOID;
    if ( pos.x >= this->width ) return BAN_VOID;
    if ( pos.y < 0 ) return BAN_VOID;
    if ( pos.y >= this->height ) return BAN_VOID;

    return this->map[pos.y][pos.x];
}

unsigned int ban_map_t::get(int x, int y) const {
    return this->get(map_position_t{x, y});
}


bool ban_map_t::is_pixel_in_water(int x, int y) const {
    return (this->get(screen_position_t{x, y + 7}) == BAN_VOID || this->get(screen_position_t{x + 15, y + 7}) == BAN_VOID)
           && (this->get(screen_position_t{x, y + 8}) == BAN_WATER || this->get(screen_position_t{x + 15, y + 8}) == BAN_WATER);
}


map_position_t ban_map_t::get_random_position() const {
    return map_position_t{rnd(this->get_width()), rnd(this->get_height())};
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

bool ban_map_t::read_from_file(const std::string& filename) {
    char chr;

    std::ifstream inputfile;
    inputfile.open(filename, std::ios_base::in|std::ios_base::binary);

    this->map.clear();

    std::vector<unsigned int> row;
    while ( inputfile.get(chr)) {
        if (chr >= '0' && chr <= '4') {
            row.push_back(chr - '0');
        } else {
            this->map.push_back(row);
            row.clear();
        }
    }
    if ( !row.empty()) {
        this->map.push_back(row);
    }

    this->width = this->map[0].size();

    this->map.emplace_back(this->width, BAN_SOLID);

    this->height = this->map.size();
    std::cout << '[' << this->width << ", " << this->height << "]\n";

    return true;

}

void ban_map_t::flip() {
    for ( auto &row : this->map )
        std::reverse(row.begin(), row.end());
}