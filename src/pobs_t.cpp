//
// Created by edwin on 10-06-18.
//

#include <cstdlib>
#include "pobs_t.h"
#include "sdl/gfx.h"

void pobs_t::init(int size) {
    this->pob_backbuf = malloc(size);

}

void pobs_t::clear() {
    this->pobs.clear();
}

void pobs_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    this->pobs.emplace_back(position, image, pob_data);
}

void pobs_t::draw() {
    int c1;
    int back_buf_ofs;

    back_buf_ofs = 0;

    for (c1 = this->pobs.size() - 1; c1 >= 0; c1--) {
        this->pobs[c1].back_buf_ofs = back_buf_ofs;
        get_block(this->pobs[c1].position.x -
                  pob_hs_x(this->pobs[c1].image, *this->pobs[c1].pob_data), this->pobs[c1].position.y -
                                                                                    pob_hs_y(this->pobs[c1].image,
                                                                                             *this->pobs[c1].pob_data),
                  pob_width(this->pobs[c1].image, *this->pobs[c1].pob_data),
                  pob_height(this->pobs[c1].image, *this->pobs[c1].pob_data),
                  (unsigned char *) this->pob_backbuf + back_buf_ofs);

        back_buf_ofs +=
                pob_width(this->pobs[c1].image, *this->pobs[c1].pob_data) *
                pob_height(this->pobs[c1].image, *this->pobs[c1].pob_data);
        put_pob(this->pobs[c1].position.x, this->pobs[c1].position.y, this->pobs[c1].image,
                *this->pobs[c1].pob_data, 1);
    }

}

void pobs_t::redraw_backgrounds() {
    int c1;

    for (c1 = 0; c1 < this->pobs.size() ; c1++)
        put_block(this->pobs[c1].position.x -
                  pob_hs_x(this->pobs[c1].image, *this->pobs[c1].pob_data), this->pobs[c1].position.y -
                                                                                    pob_hs_y(this->pobs[c1].image,
                                                                                             *this->pobs[c1].pob_data),
                  pob_width(this->pobs[c1].image, *this->pobs[c1].pob_data),
                  pob_height(this->pobs[c1].image, *this->pobs[c1].pob_data),
                  (unsigned char *) this->pob_backbuf + this->pobs[c1].back_buf_ofs);

}
