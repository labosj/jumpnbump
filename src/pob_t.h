//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_POB_T_H
#define JUMPNBUMP_POB_T_H

#include "screen_position_t.h"

struct gob_t;

struct pob_t {
    [[deprecated]]
    int x, y;

    int image;
    gob_t *pob_data;
    int back_buf_ofs;

    pob_t(const screen_position_t& position, int image, gob_t* pob_data) {
        this->x = position.x;
        this->y = position.y;
        this->image = image;
        this->pob_data = pob_data;
    }

    [[deprecated]]
    pob_t(int x, int y, int image, gob_t* pob_data) : pob_t{screen_position_t{x, y}, image, pob_data} {};
};

#endif //JUMPNBUMP_POB_T_H
