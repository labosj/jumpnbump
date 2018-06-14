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


#endif //JUMPNBUMP_GFX_H
