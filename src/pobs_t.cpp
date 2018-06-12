//
// Created by edwin on 10-06-18.
//

#include <cstdlib>
#include "pobs_t.h"
#include "sdl/gfx.h"

void pobs_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    this->pobs.emplace_back(position, image, pob_data);
}

void pobs_t::draw() {

    for (auto& pob : this->pobs) {

        put_pob(pob.position.x, pob.position.y, pob.image,
                *pob.pob_data, 1);
    }

    this->pobs.clear();

}
