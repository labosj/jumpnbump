//
// Created by edwin on 02-06-18.
//

#include "leftovers.h"
#include "gob_t.h"
#include "globals.h"
#include "draw.h"

#include "main_info.h"
#include "leftovers.h"

void draw_pobs(main_info_t &main_info) {
    int c1;
    int back_buf_ofs;

    back_buf_ofs = 0;

    for (c1 = main_info.pobs.size() - 1; c1 >= 0; c1--) {
        main_info.pobs[c1].back_buf_ofs = back_buf_ofs;
        get_block(main_info.pobs[c1].position.x -
                  pob_hs_x(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data), main_info.pobs[c1].position.y -
                                                                                    pob_hs_y(main_info.pobs[c1].image,
                                                                                             *main_info.pobs[c1].pob_data),
                  pob_width(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data),
                  pob_height(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data),
                  (unsigned char *) main_info.pob_backbuf + back_buf_ofs);

            back_buf_ofs +=
                    pob_width(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data) *
                    pob_height(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data);
        put_pob(main_info.pobs[c1].position.x, main_info.pobs[c1].position.y, main_info.pobs[c1].image,
                *main_info.pobs[c1].pob_data, 1);
    }

}

void redraw_pob_backgrounds(main_info_t &main_info) {
    int c1;

    for (c1 = 0; c1 < main_info.pobs.size() ; c1++)
        put_block(main_info.pobs[c1].position.x -
                  pob_hs_x(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data), main_info.pobs[c1].position.y -
                                                                                    pob_hs_y(main_info.pobs[c1].image,
                                                                                             *main_info.pobs[c1].pob_data),
                  pob_width(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data),
                  pob_height(main_info.pobs[c1].image, *main_info.pobs[c1].pob_data),
                  (unsigned char *) main_info.pob_backbuf + main_info.pobs[c1].back_buf_ofs);

}

int add_leftovers(const screen_position_t &position, int image, gob_t *pob_data, leftovers_t &leftovers) {

    leftovers.pobs.emplace_back(position, image, pob_data);

    return 0;

}

void draw_leftovers(leftovers_t &leftovers) {
    int c1;

    for (c1 = leftovers.pobs.size() - 1; c1 >= 0; c1--)
        put_pob(leftovers.pobs[c1].position.x, leftovers.pobs[c1].position.y, leftovers.pobs[c1].image,
                *leftovers.pobs[c1].pob_data, 1);

    leftovers.pobs.clear();

}