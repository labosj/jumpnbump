//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_DRAW_H
#define JUMPNBUMP_DRAW_H


struct gob_t;
struct main_info_t;
struct leftovers_t;

void draw_pobs(int page, main_info_t& main_info, leftovers_t& leftovers);
void redraw_pob_backgrounds(int page, main_info_t& main_info);
int add_leftovers(int page, int x, int y, int image, gob_t *pob_data, leftovers_t& leftovers);
void draw_leftovers(int page, leftovers_t& leftovers);

#endif //JUMPNBUMP_DRAW_H
