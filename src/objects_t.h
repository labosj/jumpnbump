//
// Created by edwin on 11-06-18.
//

#ifndef JUMPNBUMP_OBJECTS_T_H
#define JUMPNBUMP_OBJECTS_T_H


#include "object_t.h"


class game_manager_t;

class objects_t {
public:

    std::vector<object_t> objects;

    void update(game_manager_t& game_manager);

    void add(object_t::Type type, const position_t &position, int x_add, int y_add, int anim, int frame);

    void add_smoke(const player_t& player);
    void add_jetpack_smoke(const player_t& player);
};

extern objects_t objects;


#endif //JUMPNBUMP_OBJECTS_T_H
