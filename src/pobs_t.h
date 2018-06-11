//
// Created by edwin on 10-06-18.
//

#ifndef JUMPNBUMP_POBS_T_H
#define JUMPNBUMP_POBS_T_H

#include <vector>
#include "pob_t.h"

class pobs_t {
private:
    std::vector<pob_t> pobs;

    void *pob_backbuf = nullptr;
public:


    void add(const screen_position_t &position, int image, gob_t *pob_data);

    void redraw_backgrounds();

    void draw();

    void clear();

    void init(int size);
};


#endif //JUMPNBUMP_POBS_T_H
