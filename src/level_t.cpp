//
// Created by edwin on 05-06-18.
//

#include "level_t.h"

#include "ban_map.h"
#include "main_info.h"
#include "data.h"

extern main_info_t main_info;
extern ban_map_t ban_map;
extern int flip;
extern unsigned char *datafile_buffer;

int read_level() {
    unsigned char *handle;
    int chr;

    if ((handle = dat_open("levelmap.txt", datafile_buffer)) == 0) {
        main_info.error_str = "Error loading 'levelmap.txt', aborting...\n";
        return 1;
    }

    for (int y  = 0; y < ban_map_t::HEIGHT - 1; y++) {
        for (int x = 0; x < ban_map_t::WIDTH; x++) {
            while (true) {
                chr = (int) *(handle++);
                if (chr >= '0' && chr <= '4')
                    break;
            }
            if (flip)
                ban_map.get(ban_map_t::WIDTH - 1 - x, y) = chr - '0';
            else
                ban_map.get(x, y) = chr - '0';
        }
    }

    for (int x = 0; x < ban_map_t::WIDTH; x++)
        ban_map.get(x, ban_map_t::HEIGHT - 1) = BAN_SOLID;

    return 0;

}