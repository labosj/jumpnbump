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

    for (int c1 = this->pobs.size() - 1; c1 >= 0; c1--) {

        put_pob(this->pobs[c1].position.x, this->pobs[c1].position.y, this->pobs[c1].image,
                *this->pobs[c1].pob_data, 1);
    }

    this->pobs.clear();

}
