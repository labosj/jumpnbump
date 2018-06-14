//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_GFX_H
#define JUMPNBUMP_GFX_H

#include <string>

class pob_t;

struct gob_t;

int intr_sysupdate();

void put_pob(const pob_t& pob, int mask);
[[deprecated]]
int register_gob(unsigned char *handle, gob_t &gob, int len);



#endif //JUMPNBUMP_GFX_H
