//
// Created by edwin on 02-06-18.
//

#include "sdl/gfx.h"
#include "gob_t.h"
#include "globals.h"
#include "leftovers_t.h"
#include "screen_position_t.h"

leftovers_t leftovers;

void leftovers_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    this->pobs.emplace_back(position, image, pob_data);
}

void leftovers_t::draw() {
    for (const auto& pob : this->pobs)
        put_pob(pob, 1);

}