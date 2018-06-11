//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_LEFTOVERS_H
#define JUMPNBUMP_LEFTOVERS_H

#include <vector>
#include "pob_t.h"

class screen_position_t;

struct gob_t;

struct leftovers_t {

    std::vector<pob_t> pobs;

    void add(const screen_position_t &position, int image, gob_t *pob_data);

    void draw();
};
#endif //JUMPNBUMP_LEFTOVERS_H
