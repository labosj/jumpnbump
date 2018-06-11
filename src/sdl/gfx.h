//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_GFX_H
#define JUMPNBUMP_GFX_H

struct gob_t;

#ifdef USE_SDL
/* long filelength(int handle); */
void fs_toggle();
int intr_sysupdate();
#endif

void put_pob(int x, int y, int image, gob_t &gob, int mask);
[[deprecated]]
int pob_width(int image, gob_t& gob);
[[deprecated]]
int pob_height(int image, gob_t& gob);
[[deprecated]]
int pob_hs_x(int image, gob_t& gob);
[[deprecated]]
int pob_hs_y(int image, gob_t& gob);
[[deprecated]]
int register_gob(unsigned char *handle, gob_t &gob, int len);

void get_block(int x, int y, int width, int height, unsigned char *buffer);
void put_block(int x, int y, int width, int height, unsigned char *buffer);
#endif //JUMPNBUMP_GFX_H
