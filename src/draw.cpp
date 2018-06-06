//
// Created by edwin on 02-06-18.
//

#include "leftovers.h"
#include "gob.h"
#include "globals.h"
#include "draw.h"

#include "main_info.h"
#include "leftovers.h"

void draw_pobs(int page, main_info_t& main_info) {
    int c1;
    int back_buf_ofs;

    back_buf_ofs = 0;

    for (c1 = main_info.page_info[page].pobs.size() - 1; c1 >= 0; c1--) {
        main_info.page_info[page].pobs[c1].back_buf_ofs = back_buf_ofs;
        get_block(page, main_info.page_info[page].pobs[c1].position.x -
                        pob_hs_x(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  main_info.page_info[page].pobs[c1].position.y -
                  pob_hs_y(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  (unsigned char *) main_info.pob_backbuf[page] + back_buf_ofs);
        if (scale_up)
            back_buf_ofs +=
                    pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data) *
                    pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data) *
                    4;
        else
            back_buf_ofs +=
                    pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data) *
                    pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data);
        put_pob(page, main_info.page_info[page].pobs[c1].position.x, main_info.page_info[page].pobs[c1].position.y,
                main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data, 1);
    }

}

void redraw_pob_backgrounds(int page, main_info_t& main_info) {
    int c1;

    for (c1 = 0; c1 < main_info.page_info[page].pobs.size() ; c1++)
        put_block(page, main_info.page_info[page].pobs[c1].position.x -
                        pob_hs_x(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  main_info.page_info[page].pobs[c1].position.y -
                  pob_hs_y(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  pob_width(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  pob_height(main_info.page_info[page].pobs[c1].image, main_info.page_info[page].pobs[c1].pob_data),
                  (unsigned char *) main_info.pob_backbuf[page] + main_info.page_info[page].pobs[c1].back_buf_ofs);

}

int add_leftovers(int page, const screen_position_t&  position, int image, gob_t *pob_data, leftovers_t& leftovers) {

    leftovers.page[page].pobs.emplace_back(position, image, pob_data);

    return 0;

}

void draw_leftovers(int page, leftovers_t& leftovers) {
    int c1;

    for (c1 = leftovers.page[page].pobs.size() - 1; c1 >= 0; c1--)
        put_pob(page, leftovers.page[page].pobs[c1].position.x, leftovers.page[page].pobs[c1].position.y,
                leftovers.page[page].pobs[c1].image, leftovers.page[page].pobs[c1].pob_data, 1);

    leftovers.page[page].pobs.clear();

}