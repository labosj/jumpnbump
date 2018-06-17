//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_LEFTOVERS_H
#define JUMPNBUMP_LEFTOVERS_H

#include <vector>
#include "pob_t.h"

class screen_position_t;
class game_manager_t;

struct gob_t;

//this are the red gibs of the rabbits
class leftovers_t {
private:
    std::vector<pob_t> pobs;

public:
    void add(const screen_position_t &position, int image, gob_t *pob_data);

    void draw(game_manager_t& game_manager);
};
#endif //JUMPNBUMP_LEFTOVERS_H
