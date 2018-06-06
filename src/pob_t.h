//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_POB_T_H
#define JUMPNBUMP_POB_T_H

#include "screen_position_t.h"

struct gob_t;

/**
 * Maybe this stands for player object
 */
struct pob_t {
    screen_position_t position;

    int image;
    gob_t *pob_data;
    int back_buf_ofs;

    pob_t(const screen_position_t& position, int image, gob_t* pob_data) {
        this->position = position;
        this->image = image;
        this->pob_data = pob_data;
    }
};


int add_pob(int page, const screen_position_t& position, int image, gob_t *pob_data);

#endif //JUMPNBUMP_POB_T_H
