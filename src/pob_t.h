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
};

#endif //JUMPNBUMP_POB_T_H
