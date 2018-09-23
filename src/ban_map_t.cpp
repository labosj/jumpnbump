//
// Created by edwin on 02-06-18.
//

#include <fstream>
#include <utility>
#include <algorithm>
#include <iostream>
#include "ban_map_t.h"
#include "screen_position_t.h"
#include "util.h"

ban_map_t::Type ban_map_t::get(const map_position_t& pos) const {
    //std::cout << "const[" << pos.x << "," << pos.y << "]\n";
    if ( pos.x.value < 0) return ban_map_t::Type::VOID;
    if ( pos.x.value >= this->width ) return ban_map_t::Type::VOID;
    if ( pos.y.value < 0 ) return ban_map_t::Type::VOID;
    if ( pos.y.value >= this->height ) return ban_map_t::Type::VOID;

    return this->map[pos.y.value][pos.x.value];
}

bool ban_map_t::is_in_water(const screen_position_t& position) const {
    return (this->get(position + screen_position_t{0,7}) == ban_map_t::Type::VOID || this->get(position + screen_position_t{15, 7}) == ban_map_t::Type::VOID)
           && (this->get(position + screen_position_t{0, 8}) == ban_map_t::Type::WATER || this->get(position + screen_position_t{15, 8}) == ban_map_t::Type::WATER);
}

bool ban_map_t::is_solid(const screen_position_t& position) const {
    return  this->get(position) == ban_map_t::Type::SOLID ||
            this->get(position) == ban_map_t::Type::ICE ||
            this->get(position) == ban_map_t::Type::SPRING;
}

map_position_t ban_map_t::get_random_position() const {
    return map_position_t{rnd(this->get_width()), rnd(this->get_height())};
};

map_position_t ban_map_t::get_random_available_position() const {

    while (true) {
        auto position = get_random_position();

        if (
                this->get(position) == ban_map_t::Type::VOID            //si esta vacio
                ) {
            return position;
        }
    }
}

map_position_t ban_map_t::get_random_available_floor_position() const {

    while (true) {
        auto position = get_random_available_position();
        auto below = position;
        below.y.value += 1;

        if (
               this->get(below) == ban_map_t::Type::SOLID ||     //y el de abajo es un solido, posiblemente un suelo
               this->get(below) == ban_map_t::Type::ICE  //o un hielo
         )
            return position;
    }
}

bool ban_map_t::read_from_file(const std::string& filename) {
    char chr;

    std::ifstream inputfile;
    inputfile.open(filename, std::ios_base::in|std::ios_base::binary);

    this->map.clear();

    decltype(this->map)::value_type row;
    while ( inputfile.get(chr)) {
        if (chr >= '0' && chr <= '4') {
            row.push_back(static_cast<Type>(chr - '0'));
        } else {
            this->map.push_back(row);
            row.clear();
        }
    }
    if ( !row.empty()) {
        this->map.push_back(row);
    }

    this->width = this->map[0].size();

    this->map.emplace_back(this->width, ban_map_t::Type::SOLID);

    this->height = this->map.size();
    std::cout << '[' << this->width << ", " << this->height << "]\n";

    return true;

}