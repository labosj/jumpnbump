//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_GFX_H
#define JUMPNBUMP_GFX_H

#include <string>

class pob_t;

struct gob_t;

#ifdef USE_SDL
void fs_toggle();
int intr_sysupdate();
#endif

void put_pob(const pob_t& pob, int mask);
[[deprecated]]
int register_gob(unsigned char *handle, gob_t &gob, int len);


void open_screen(void);
void draw_begin(void);
void draw_end(void);
void flippage();
void draw_begin(void);
void draw_end(void);
void setpalette(int index, int count, char *palette);
void put_block(int x, int y, int width, int height, unsigned char *buffer);

int read_pcx(const std::string& filename, unsigned char *buffer, int buf_len, char *pal);
void register_background(unsigned char *pixels);

void register_mask(void *pixels);

#endif //JUMPNBUMP_GFX_H
