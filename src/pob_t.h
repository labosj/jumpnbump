//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_POB_T_H
#define JUMPNBUMP_POB_T_H

struct gob_t;

struct pob_t {
    int x, y;
    int image;
    gob_t *pob_data;
    int back_buf_ofs;

    pob_t(int x, int y, int image, gob_t* pob_data) {
        this->x = x;
        this->y = y;
        this->image = image;
        this->pob_data = pob_data;
    }
};

#endif //JUMPNBUMP_POB_T_H
