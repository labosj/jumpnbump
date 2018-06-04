/*
 * main.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * Copyright (C) 2015 Côme Chilliet <come@chilliet.eu>
 *
 * This file is part of Jump 'n Bump.
 *
 * Jump 'n Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump 'n Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "globals.h"
#include "gob.h"
#include "object_anim.h"
#include "leftovers.h"
#include "util.h"
#include "draw.h"
#include "player.h"
#include "menu.h"
#include "data.h"
#include "ban_map.h"
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include "sdl/gfx.h"
#include "object_t.h"
#include "player_anim_t.h"
#include "joy_t.h"


#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
int endscore_reached;


const auto RABBIT_NAME_1 = "AMANDA";
const auto RABBIT_NAME_2 = "EDWIN";
unsigned char *datafile_buffer = nullptr;

gob_t rabbit_gobs = {0};
gob_t font_gobs = {0};
gob_t object_gobs = {0};
gob_t number_gobs = {0};

main_info_t main_info;
std::vector<player_t> players{JNB_MAX_PLAYERS};
std::vector<player_anim_t> player_anims{7};
std::vector<object_t> objects;
joy_t joy;

char datfile_name[2048];

unsigned char *background_pic;
unsigned char *mask_pic;
int flip = 0;
char pal[768];
char cur_pal[768];


object_anim_t object_anims[8] = {
        {
                6,  0,
                {
                        {0,  3},
                        {1,  3},
                        {2,  3},
                        {3,  3},
                        {4,  3},
                        {5,  3},
                        {0,  0},
                        {0,  0},
                        {0,  0},
                        {0,  0}
                }
        },
        {
                9,  0,
                {
                        {6,  2},
                        {7,  2},
                        {8,  2},
                        {9,  2},
                        {10, 2},
                        {11, 2},
                        {12, 2},
                        {13, 2},
                        {14, 2},
                        {0,  0}
                }
        },
        {
                5,  0,
                {
                        {15, 3},
                        {16, 3},
                        {16, 3},
                        {17, 3},
                        {18, 3},
                        {19, 3},
                        {0,  0},
                        {0,  0},
                        {0,  0},
                        {0,  0}
                }
        },
        {
                10, 0,
                {
                        {20, 2},
                        {21, 2},
                        {22, 2},
                        {23, 2},
                        {24, 2},
                        {25, 2},
                        {24, 2},
                        {23, 2},
                        {22, 2},
                        {21, 2}
                }
        },
        {
                10, 0,
                {
                        {26, 2},
                        {27, 2},
                        {28, 2},
                        {29, 2},
                        {30, 2},
                        {31, 2},
                        {30, 2},
                        {29, 2},
                        {28, 2},
                        {27, 2}
                }
        },
        {
                10, 0,
                {
                        {32, 2},
                        {33, 2},
                        {34, 2},
                        {35, 2},
                        {36, 2},
                        {37, 2},
                        {36, 2},
                        {35, 2},
                        {34, 2},
                        {33, 2}
                }
        },
        {
                10, 0,
                {
                        {38, 2},
                        {39, 2},
                        {40, 2},
                        {41, 2},
                        {42, 2},
                        {43, 2},
                        {42, 2},
                        {41, 2},
                        {40, 2},
                        {39, 2}
                }
        },
        {
                4,  0,
                {
                        {76, 4},
                        {77, 4},
                        {78, 4},
                        {79, 4},
                        {0,  0},
                        {0,  0},
                        {0,  0},
                        {0,  0},
                        {0,  0},
                        {0,  0}
                }
        }
};

leftovers_t leftovers;

int pogostick, bunnies_in_space, jetpack, blood_is_thicker_than_water;

int client_player_num = -1;


static void flip_pixels(unsigned char *pixels) {
    int x, y;
    unsigned char temp;

    assert(pixels);
    for (y = 0; y < JNB_HEIGHT; y++) {
        for (x = 0; x < (352 / 2); x++) {
            temp = pixels[y * JNB_WIDTH + x];
            pixels[y * JNB_WIDTH + x] = pixels[y * JNB_WIDTH + (352 - x) - 1];
            pixels[y * JNB_WIDTH + (352 - x) - 1] = temp;
        }
    }
}


void serverSendKillPacket(int killer, int victim) {
    int c1 = killer;
    int c2 = victim;
    int x = players[victim].x;
    int y = players[victim].y;
    int c4 = 0;
    int s1 = 0;

    players[c1].y_add = -players[c1].y_add;
    if (players[c1].y_add > -262144L)
        players[c1].y_add = -262144L;
    players[c1].jump_abort = 1;
    players[c2].dead_flag = 1;
    if (players[c2].anim != 6) {
        players[c2].anim = 6;
        players[c2].frame = 0;
        players[c2].frame_tick = 0;
        players[c2].image = player_anims[players[c2].anim].frame[players[c2].frame].image + players[c2].direction * 9;
        if (main_info.no_gore == 0) {
            for (c4 = 0; c4 < 6; c4++)
                add_object(OBJ_FUR, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 44 + c2 * 8);
            for (c4 = 0; c4 < 6; c4++)
                add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 76);
            for (c4 = 0; c4 < 6; c4++)
                add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 77);
            for (c4 = 0; c4 < 8; c4++)
                add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 78);
            for (c4 = 0; c4 < 10; c4++)
                add_object(OBJ_FLESH, (x >> 16) + 6 + rnd(5), (y >> 16) + 6 + rnd(5), (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 79);
        }
        dj_play_sfx(main_info, SFX_DEATH, (unsigned short) (SFX_DEATH_FREQ + rnd(2000) - 1000), 64, 0, -1);


        players[c1].count_kill(c2);
        if (players[c1].bumps >= JNB_END_SCORE) {
            endscore_reached = 1;
        }
        s1 = players[c1].bumps % 100;
        add_leftovers(0, 360, 34 + c1 * 64, s1 / 10, &number_gobs, leftovers);
        add_leftovers(1, 360, 34 + c1 * 64, s1 / 10, &number_gobs, leftovers);
        add_leftovers(0, 376, 34 + c1 * 64, s1 - (s1 / 10) * 10, &number_gobs, leftovers);
        add_leftovers(1, 376, 34 + c1 * 64, s1 - (s1 / 10) * 10, &number_gobs, leftovers);
    }
}

void set_blood_is_thicker_than_water() {
    char blood[32] = {
            63, 32, 32, 53, 17, 17, 42, 7,
            7, 28, 0, 0, 24, 0, 0, 19,
            0, 0, 12, 0, 0, 7, 0, 0
    };
    char water[32] = {
            63, 63, 63, 40, 53, 62, 19, 42,
            60, 0, 33, 60, 3, 32, 46, 3,
            26, 33, 3, 19, 21, 1, 8, 8
    };
    int i;

    blood_is_thicker_than_water ^= 1;
    if (blood_is_thicker_than_water == 1) {
        for (i = 0; i < 32; i++)
            pal[432 + i] = blood[i];
    } else {
        for (i = 0; i < 32; i++)
            pal[432 + i] = water[i];
    }
    register_background(background_pic);
}


static void game_loop(void) {
    int mod_vol, sfx_vol;
    int update_count = 1;
    int end_loop_flag = 0;
    int fade_flag = 0;
    int update_palette = 0;
    int mod_fade_direction;
    int i;

    mod_vol = sfx_vol = 0;
    mod_fade_direction = 1;
    dj_ready_mod(main_info, MOD_GAME, datafile_buffer);
    dj_set_mod_volume(main_info, (char) mod_vol);
    dj_set_sfx_volume(main_info, (char) mod_vol);
    dj_start_mod(main_info);

    intr_sysupdate();

    endscore_reached = 0;

    //set_blood_is_thicker_than_water();
    while (1) {
        while (update_count) {

            if (endscore_reached || (key_pressed(1) == 1)) {

                end_loop_flag = 1;
                memset(pal, 0, 768);
                mod_fade_direction = 0;
            }


            steer_players();



            collision_check();


            main_info.page_info[main_info.draw_page].num_pobs = 0;
            for (i = 0; i < players.size(); i++) {
                if (players[i].enabled == 1)
                    main_info.page_info[main_info.draw_page].num_pobs++;
            }

            update_objects();


            if (update_count == 1) {
                int c2;

                for (i = 0, c2 = 0; i < players.size(); i++) {
                    if (players[i].enabled == 1) {
                        main_info.page_info[main_info.draw_page].pobs[c2].x = players[i].x >> 16;
                        main_info.page_info[main_info.draw_page].pobs[c2].y = players[i].y >> 16;
                        main_info.page_info[main_info.draw_page].pobs[c2].image = players[i].image + i * 18;
                        main_info.page_info[main_info.draw_page].pobs[c2].pob_data = &rabbit_gobs;
                        c2++;
                    }
                }

                draw_begin();

                draw_pobs(main_info.draw_page, main_info);


                draw_end();
            }

            if (mod_fade_direction == 1) {
                if (mod_vol < 30) {
                    mod_vol++;
                    dj_set_mod_volume(main_info, (char) mod_vol);
                }
                if (sfx_vol < 64) {
                    sfx_vol++;
                    dj_set_sfx_volume(main_info, (char) sfx_vol);
                }
            } else {
                if (mod_vol > 0) {
                    mod_vol--;
                    dj_set_mod_volume(main_info, (char) mod_vol);
                }
                if (sfx_vol > 0) {
                    sfx_vol--;
                    dj_set_sfx_volume(main_info, (char) sfx_vol);
                }
            }

            fade_flag = 0;
            for (i = 0; i < 768; i++) {
                if (cur_pal[i] < pal[i]) {
                    cur_pal[i]++;
                    fade_flag = 1;
                } else if (cur_pal[i] > pal[i]) {
                    cur_pal[i]--;
                    fade_flag = 1;
                }
            }
            if (fade_flag == 1)
                update_palette = 1;
            if (fade_flag == 0 && end_loop_flag == 1)
                break;

            if (update_count == 1) {
                if (update_palette == 1) {
                    setpalette(0, 256, cur_pal);
                    update_palette = 0;
                }

                main_info.draw_page ^= 1;
                main_info.view_page ^= 1;

                flippage(main_info.view_page);

                wait_vrt();

                draw_begin();

                redraw_pob_backgrounds(main_info.draw_page, main_info);

                draw_leftovers(main_info.draw_page, leftovers);

                draw_end();
            }

            update_count--;
        }

        update_count = intr_sysupdate();

        if ((fade_flag == 0) && (end_loop_flag == 1))
            break;
    }
}


static int menu_loop(unsigned char* datafile_buffer) {
    unsigned char *handle;
    int mod_vol;
    int c1, c2;

    while (true) {

        if (menu(main_info, datafile_buffer, leftovers) != 0)
            deinit_program();


        if (key_pressed(1) == 1) {
            return 0;
        }
        if (init_level(0, pal) != 0) {
            deinit_level();
            deinit_program();
        }

        memset(cur_pal, 0, 768);
        setpalette(0, 256, cur_pal);


        flippage(1);
        register_background(background_pic);
        flippage(0);

        bunnies_in_space = jetpack = pogostick = blood_is_thicker_than_water = 0;
        //blood_is_thicker_than_water = 1; HERE IS TO MOD THE CHEATS
        main_info.page_info[0].num_pobs = 0;
        main_info.page_info[1].num_pobs = 0;
        main_info.view_page = 0;
        main_info.draw_page = 1;

        game_loop();

        main_info.view_page = 0;
        main_info.draw_page = 1;

        dj_stop_sfx_channel(main_info, 4);

        deinit_level();

        memset(mask_pic, 0, JNB_WIDTH * JNB_HEIGHT);
        register_mask(mask_pic);

        register_background(NULL);

        draw_begin();

        put_text(main_info.view_page, 100, 50, RABBIT_NAME_1, 2);
        put_text(main_info.view_page, 160, 50, RABBIT_NAME_2, 2);
        put_text(main_info.view_page, 220, 50, "FIZZ", 2);
        put_text(main_info.view_page, 280, 50, "MIJJI", 2);
        put_text(main_info.view_page, 40, 80, RABBIT_NAME_1, 2);
        put_text(main_info.view_page, 40, 110, RABBIT_NAME_2, 2);
        put_text(main_info.view_page, 40, 140, "FIZZ", 2);
        put_text(main_info.view_page, 40, 170, "MIJJI", 2);

        for (c1 = 0; c1 < players.size(); c1++) {
            if (!players[c1].enabled) {
                continue;
            }
            char str1[100];

            for (c2 = 0; c2 < players.size(); c2++) {
                if (!players[c2].enabled) {
                    continue;
                }
                if (c2 != c1) {
                    sprintf(str1, "%d", players[c1].bumped[c2]);
                    put_text(main_info.view_page, 100 + c2 * 60, 80 + c1 * 30, str1, 2);
                } else
                    put_text(main_info.view_page, 100 + c2 * 60, 80 + c1 * 30, "-", 2);
            }
            sprintf(str1, "%d", players[c1].bumps);
            put_text(main_info.view_page, 350, 80 + c1 * 30, str1, 2);
        }

        put_text(main_info.view_page, 200, 230, "Press ESC to continue", 2);

        draw_end();

        flippage(main_info.view_page);

        if ((handle = dat_open("menu.pcx", datafile_buffer)) == 0) {
            main_info.error_str = "Error loading 'menu.pcx', aborting...\n";
            return 1;
        }
        if (read_pcx(handle, background_pic, JNB_WIDTH * JNB_HEIGHT, pal) != 0) {
            main_info.error_str = "Error loading 'menu.pcx', aborting...\n";
            return 1;
        }

        /* fix dark font */
        for (c1 = 0; c1 < 16; c1++) {
            pal[(240 + c1) * 3 + 0] = c1 << 2;
            pal[(240 + c1) * 3 + 1] = c1 << 2;
            pal[(240 + c1) * 3 + 2] = c1 << 2;
        }

        memset(cur_pal, 0, 768);

        setpalette(0, 256, cur_pal);

        mod_vol = 0;
        dj_ready_mod(main_info, MOD_SCORES, datafile_buffer);
        dj_set_mod_volume(main_info, (char) mod_vol);
        dj_start_mod(main_info);

        while (key_pressed(1) == 0) {
            if (mod_vol < 35)
                mod_vol++;
            dj_set_mod_volume(main_info, (char) mod_vol);
            for (c1 = 0; c1 < 768; c1++) {
                if (cur_pal[c1] < pal[c1])
                    cur_pal[c1]++;
            }

            intr_sysupdate();
            wait_vrt();
            setpalette(0, 256, cur_pal);
            flippage(main_info.view_page);
        }
        while (key_pressed(1) == 1) {

            intr_sysupdate();
        }

        memset(pal, 0, 768);

        while (mod_vol > 0) {
            mod_vol--;
            dj_set_mod_volume(main_info, (char) mod_vol);
            for (c1 = 0; c1 < 768; c1++) {
                if (cur_pal[c1] > pal[c1])
                    cur_pal[c1]--;
            }

            wait_vrt();
            setpalette(0, 256, cur_pal);
            flippage(main_info.view_page);
        }

        fillpalette(0, 0, 0);


        dj_stop_mod(main_info);
    }
}


int main(int argc, char *argv[]) {
    int result;

    if (init_program(argc, argv, pal) != 0)
        deinit_program();

    result = menu_loop(datafile_buffer);

    deinit_program();

    return result;
}


void add_object(int type, int x, int y, int x_add, int y_add, int anim, int frame) {

    for ( auto& object : objects ) {
        if (object.used == 0) {
            object = object_t{type, x, y, x_add, y_add, anim, frame};
            return;
        }
    }

    objects.emplace_back(type, x, y, x_add, y_add, anim, frame);

}


void update_objects(void) {

    for (auto& object : objects) {

        if (object.is_used()) {
            switch (object.type) {
                case OBJ_SPRING:
                    object.update_spring();
                    if (object.is_used() )
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.image,
                                &object_gobs);
                    break;
                case OBJ_SPLASH:
                    object.update_splash();
                    if (object.is_used() )
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.image,
                                &object_gobs);
                    break;
                case OBJ_SMOKE:
                    object.update_smoke();
                    if (object.is_used() )
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.image,
                                &object_gobs);
                    break;
                case OBJ_YEL_BUTFLY:
                case OBJ_PINK_BUTFLY:
                    object.update_butterfly();
                    if (object.is_used() )
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.image,
                                &object_gobs);
                    break;
                case OBJ_FUR:
                    object.update_fur();
                    if (object.is_used() ) {
                        int s1 = (int) (atan2(object.y_add, object.x_add) * 4 / M_PI);
                        if (s1 < 0)
                            s1 += 8;
                        if (s1 < 0)
                            s1 = 0;
                        if (s1 > 7)
                            s1 = 7;
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.frame + s1,
                                &object_gobs);
                    }
                    break;
                case OBJ_FLESH:
                    object.update_flesh();
                    if (object.is_used())
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.frame,
                                &object_gobs);
                    break;
                case OBJ_FLESH_TRACE:
                    object.update_flesh_trace();

                    if (object.is_used() )
                        add_pob(main_info.draw_page, object.x >> 16, object.y >> 16, object.image,
                                &object_gobs);
                    break;
            }
        }
    }

}


int add_pob(int page, int x, int y, int image, gob_t *pob_data) {

    if (main_info.page_info[page].num_pobs >= main_info_t::NUM_POBS)
        return 1;

    main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].x = x;
    main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].y = y;
    main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].image = image;
    main_info.page_info[page].pobs[main_info.page_info[page].num_pobs].pob_data = pob_data;
    main_info.page_info[page].num_pobs++;

    return 0;

}


int init_level(int level, char *pal) {
    unsigned char *handle;
    int c1, c2;
    int s1, s2;

    if ((handle = dat_open("level.pcx", datafile_buffer)) == 0) {
        main_info.error_str = "Error loading 'level.pcx', aborting...\n";
        return 1;
    }
    if (read_pcx(handle, background_pic, JNB_WIDTH * JNB_HEIGHT, pal) != 0) {
        main_info.error_str = "Error loading 'level.pcx', aborting...\n";
        return 1;
    }
    if (flip)
        flip_pixels(background_pic);
    if ((handle = dat_open("mask.pcx", datafile_buffer)) == 0) {
        main_info.error_str = "Error loading 'mask.pcx', aborting...\n";
        return 1;
    }
    if (read_pcx(handle, mask_pic, JNB_WIDTH * JNB_HEIGHT, 0) != 0) {
        main_info.error_str = "Error loading 'mask.pcx', aborting...\n";
        return 1;
    }
    if (flip)
        flip_pixels(mask_pic);
    register_mask(mask_pic);

    for (c1 = 0; c1 < players.size(); c1++) {
        if (players[c1].enabled == 1) {
            players[c1].reset_kills();
            position_player(c1);
            add_leftovers(0, 360, 34 + c1 * 64, 0, &number_gobs, leftovers);
            add_leftovers(1, 360, 34 + c1 * 64, 0, &number_gobs, leftovers);
            add_leftovers(0, 376, 34 + c1 * 64, 0, &number_gobs, leftovers);
            add_leftovers(1, 376, 34 + c1 * 64, 0, &number_gobs, leftovers);
        }
    }

    for (c1 = 0; c1 < objects.size(); c1++)
        objects[c1].used = 0;

    for (c1 = 0; c1 < 16; c1++) {
        for (c2 = 0; c2 < 22; c2++) {
            if (ban_map.get(c2, c1) == BAN_SPRING)
                add_object(OBJ_SPRING, c2 << 4, c1 << 4, 0, 0, OBJ_ANIM_SPRING, 5);
        }
    }

    for ( int i = 0 ; i < 2 ; i++ ) {
        auto new_pos = ban_map.get_random_available_position();
        add_object(OBJ_YEL_BUTFLY, (new_pos.first << 4) + 8, (new_pos.second << 4) + 8, (rnd(65535) - 32768) * 2,
                   (rnd(65535) - 32768) * 2,
                   0, 0);
    }

    for ( int i = 0 ; i < 2 ; i++ ) {
        auto new_pos = ban_map.get_random_available_position();
        add_object(OBJ_PINK_BUTFLY, (new_pos.first << 4) + 8, (new_pos.second << 4) + 8, (rnd(65535) - 32768) * 2,
                   (rnd(65535) - 32768) * 2, 0, 0);
    }


    return 0;

}


void deinit_level(void) {

    dj_stop_mod(main_info);
}


#ifndef PATH_MAX
#define PATH_MAX 1024
#endif
#ifndef O_BINARY
#define O_BINARY 0
#endif

static void preread_datafile(const std::string& fname) {
    int fd = 0;
    int len;


    fd = open(fname.c_str(), O_RDONLY | O_BINARY);
    if (fd == -1) {
        fprintf(stderr, "can't open %s:", fname);
        perror("");
        exit(42);
    }

    len = filelength(fd);
    datafile_buffer = (unsigned char *) malloc(len);
    if (datafile_buffer == nullptr) {
        perror("malloc()");
        close(fd);
        exit(42);
    }

    if (read(fd, datafile_buffer, len) != len) {
        perror("read()");
        close(fd);
        exit(42);
    }

    close(fd);
}


int init_program(int argc, char *argv[], char *pal) {
    unsigned char *handle = (unsigned char *) NULL;
    int c1 = 0, c2 = 0;
    int load_flag = 0;
    main_info.music_no_sound =0;
    main_info.no_sound = 0;
    int player_anim_data[] = {
            1, 0, 0, 0x7fff, 0, 0, 0, 0, 0, 0,
            4, 0, 0, 4, 1, 4, 2, 4, 3, 4,
            1, 0, 4, 0x7fff, 0, 0, 0, 0, 0, 0,
            4, 2, 5, 8, 6, 10, 7, 3, 6, 3,
            1, 0, 6, 0x7fff, 0, 0, 0, 0, 0, 0,
            2, 1, 5, 8, 4, 0x7fff, 0, 0, 0, 0,
            1, 0, 8, 5, 0, 0, 0, 0, 0, 0
    };

    srand(time(NULL));

    if (hook_keyb_handler() != 0)
        return 1;

    //memset(&main_info, 0, sizeof(main_info));

    std::string datfile_name = "/home/edwin/Projects/jumpnbump/data/jumpbump.dat";

    if (argc > 1) {
        for (c1 = 1; c1 < argc; c1++) {
            if (stricmp(argv[c1], "-nosound") == 0)
                main_info.no_sound = 1;
            else if (stricmp(argv[c1], "-musicnosound") == 0)
                main_info.music_no_sound = 1;
            else if (stricmp(argv[c1], "-nomusic") == 0);
            else if (stricmp(argv[c1], "-nogore") == 0)
                main_info.no_gore = 1;
            else if (stricmp(argv[c1], "-nojoy") == 0)
                main_info.joy_enabled = 0;
            else if (stricmp(argv[c1], "-fullscreen") == 0)
                fs_toggle();
            else if (stricmp(argv[c1], "-scaleup") == 0)
                set_scaling(1);
            else if (stricmp(argv[c1], "-mirror") == 0)
                flip = 1;
             else if (stricmp(argv[c1], "-players") == 0) {
                if (c1 < (argc - 1)) {
                    if (client_player_num < 0)
                        client_player_num = atoi(argv[c1 + 1]);
                }
            } else if (strstr(argv[1], "-v")) {
                printf("jumpnbump %s compiled with", JNB_VERSION);
                printf(" network support.\n");
                return 1;
            } else if (strstr(argv[1], "-h")) {
                printf("Usage: jumpnbump [OPTION]...\n");
                printf("\n");
                printf("  -h                       this help\n");
                printf("  -v                       print version\n");
                printf("  -dat level.dat           play a different level\n");
                printf("  -players num              set main players to num (0-3). Needed for networking\n");
                printf("  -fullscreen              run in fullscreen mode\n");
                printf("  -nosound                 play without sound\n");
                printf("  -nogore                  play without blood\n");
                printf("  -mirror                  play with mirrored level\n");
                printf("  -scaleup                 play with doubled resolution (800x512)\n");
                printf("  -musicnosound            play with music but without sound\n");
                printf("\n");
                return 1;
            }
        }
    }

    preread_datafile(datfile_name);

    main_info.pob_backbuf[0] = malloc(screen_pitch * screen_height);
    main_info.pob_backbuf[1] = malloc(screen_pitch * screen_height);

    for (c1 = 0; c1 < 7; c1++) {
        player_anims[c1].num_frames = player_anim_data[c1 * 10];
        player_anims[c1].restart_frame = player_anim_data[c1 * 10 + 1];
        for (c2 = 0; c2 < 4; c2++) {
            player_anims[c1].frame[c2].image = player_anim_data[c1 * 10 + c2 * 2 + 2];
            player_anims[c1].frame[c2].ticks = player_anim_data[c1 * 10 + c2 * 2 + 3];
        }
    }

    if ((handle = dat_open("menu.pcx", datafile_buffer)) == nullptr) {
        main_info.error_str = "Error loading 'menu.pcx', aborting...\n";
        return 1;
    }
    if (read_pcx(handle, background_pic, JNB_WIDTH * JNB_HEIGHT, pal) != 0) {
        main_info.error_str = "Error loading 'menu.pcx', aborting...\n";
        return 1;
    }

    if ((handle = dat_open("rabbit.gob", datafile_buffer)) == nullptr) {
        main_info.error_str = "Error loading 'rabbit.gob', aborting...\n";
        return 1;
    }
    if (register_gob(handle, &rabbit_gobs, dat_filelen("rabbit.gob", datafile_buffer))) {
        /* error */
        return 1;
    }

    if ((handle = dat_open("objects.gob", datafile_buffer)) == nullptr) {
        main_info.error_str = "Error loading 'objects.gob', aborting...\n";
        return 1;
    }
    if (register_gob(handle, &object_gobs, dat_filelen("objects.gob", datafile_buffer))) {
        /* error */
        return 1;
    }

    if ((handle = dat_open("font.gob", datafile_buffer)) == nullptr) {
        main_info.error_str = "Error loading 'font.gob', aborting...\n";
        return 1;
    }
    if (register_gob(handle, &font_gobs, dat_filelen("font.gob", datafile_buffer))) {
        /* error */
        return 1;
    }

    if ((handle = dat_open("numbers.gob", datafile_buffer)) == nullptr) {
        main_info.error_str = "Error loading 'numbers.gob', aborting...\n";
        return 1;
    }
    if (register_gob(handle, &number_gobs, dat_filelen("numbers.gob", datafile_buffer))) {
        /* error */
        return 1;
    }

    if (read_level() != 0) {
        main_info.error_str = "Error loading 'levelmap.txt', aborting...\n";
        return 1;
    }

    dj_init(main_info);

    if (main_info.no_sound == 0) {

        dj_set_mixing_freq(20000);

        dj_set_num_sfx_channels(5);
        dj_set_sfx_volume(main_info, 64);

        if ((handle = dat_open("jump.smp", datafile_buffer)) == 0) {
            main_info.error_str = "Error loading 'jump.smp', aborting...\n";
            return 1;
        }
        if (dj_load_sfx(main_info, handle, 0, dat_filelen("jump.smp", datafile_buffer), SFX_JUMP) != 0) {
            main_info.error_str = "Error loading 'jump.smp', aborting...\n";
            return 1;
        }

        if ((handle = dat_open("death.smp", datafile_buffer)) == 0) {
            main_info.error_str = "Error loading 'death.smp', aborting...\n";
            return 1;
        }
        if (dj_load_sfx(main_info, handle, 0, dat_filelen("death.smp", datafile_buffer), SFX_DEATH) != 0) {
            main_info.error_str = "Error loading 'death.smp', aborting...\n";
            return 1;
        }

        if ((handle = dat_open("spring.smp", datafile_buffer)) == 0) {
            main_info.error_str = "Error loading 'spring.smp', aborting...\n";
            return 1;
        }
        if (dj_load_sfx(main_info, handle, 0, dat_filelen("spring.smp", datafile_buffer), SFX_SPRING) != 0) {
            main_info.error_str = "Error loading 'spring.smp', aborting...\n";
            return 1;
        }

        if ((handle = dat_open("splash.smp", datafile_buffer)) == 0) {
            main_info.error_str = "Error loading 'splash.smp', aborting...\n";
            return 1;
        }
        if (dj_load_sfx(main_info, handle, 0, dat_filelen("splash.smp", datafile_buffer), SFX_SPLASH) != 0) {
            main_info.error_str = "Error loading 'splash.smp', aborting...\n";
            return 1;
        }
    }

    if ((background_pic = reinterpret_cast<unsigned char *>(malloc(JNB_WIDTH * JNB_HEIGHT))) == nullptr)
        return 1;
    if ((mask_pic = reinterpret_cast<unsigned char *>(malloc(JNB_WIDTH * JNB_HEIGHT))) == nullptr)
        return 1;
    memset(mask_pic, 0, JNB_WIDTH * JNB_HEIGHT);
    register_mask(mask_pic);

    /* fix dark font */
    for (c1 = 0; c1 < 16; c1++) {
        pal[(240 + c1) * 3 + 0] = c1 << 2;
        pal[(240 + c1) * 3 + 1] = c1 << 2;
        pal[(240 + c1) * 3 + 2] = c1 << 2;
    }

    setpalette(0, 256, pal);

    init_inputs(main_info);


    if (main_info.joy_enabled == 1) {
        load_flag = 0;
        put_text(0, 200, 40, "JOYSTICK CALIBRATION", 2);
        put_text(0, 200, 100, "Move the joystick to the", 2);
        put_text(0, 200, 115, "UPPER LEFT", 2);
        put_text(0, 200, 130, "and press button A", 2);
        put_text(0, 200, 200, "Or press ESC to use", 2);
        put_text(0, 200, 215, "previous settings", 2);
        if (calib_joy() != 0)
            load_flag = 1;
        else {
            register_background(NULL);

            main_info.view_page = 1;
            flippage(1);

            wait_vrt();

            put_text(1, 200, 40, "JOYSTICK CALIBRATION", 2);
            put_text(1, 200, 100, "Move the joystick to the", 2);
            put_text(1, 200, 115, "LOWER RIGHT", 2);
            put_text(1, 200, 130, "and press button A", 2);
            put_text(1, 200, 200, "Or press ESC to use", 2);
            put_text(1, 200, 215, "previous settings", 2);
            if (calib_joy() != 0)
                load_flag = 1;
            else {
                register_background(NULL);
                flippage(0);

                wait_vrt();

                put_text(0, 200, 40, "JOYSTICK CALIBRATION", 2);
                put_text(0, 200, 100, "Move the joystick to the", 2);
                put_text(0, 200, 115, "CENTER", 2);
                put_text(0, 200, 130, "and press button A", 2);
                put_text(0, 200, 200, "Or press ESC to use", 2);
                put_text(0, 200, 215, "previous settings", 2);
                if (calib_joy() != 0)
                    load_flag = 1;
                else {
                    if (joy.calib_data.x1 == joy.calib_data.x2)
                        joy.calib_data.x1 -= 10;
                    if (joy.calib_data.x3 == joy.calib_data.x2)
                        joy.calib_data.x3 += 10;
                    if (joy.calib_data.y1 == joy.calib_data.y2)
                        joy.calib_data.y1 -= 10;
                    if (joy.calib_data.y3 == joy.calib_data.y2)
                        joy.calib_data.y3 += 10;
                    write_calib_data();
                }
            }
        }
        if (load_flag == 1) {
            if ((handle = dat_open("calib.dat", datafile_buffer)) == 0) {
                main_info.error_str = "Error loading 'calib.dat', aborting...\n";
                return 1;
            }
            joy.calib_data.x1 = (handle[0]) + (handle[1] << 8) + (handle[2] << 16) + (handle[3] << 24);
            handle += 4;
            joy.calib_data.x2 = (handle[0]) + (handle[1] << 8) + (handle[2] << 16) + (handle[3] << 24);
            handle += 4;
            joy.calib_data.x3 = (handle[0]) + (handle[1] << 8) + (handle[2] << 16) + (handle[3] << 24);
            handle += 4;
            joy.calib_data.y1 = (handle[0]) + (handle[1] << 8) + (handle[2] << 16) + (handle[3] << 24);
            handle += 4;
            joy.calib_data.y2 = (handle[0]) + (handle[1] << 8) + (handle[2] << 16) + (handle[3] << 24);
            handle += 4;
            joy.calib_data.y3 = (handle[0]) + (handle[1] << 8) + (handle[2] << 16) + (handle[3] << 24);
            handle += 4;
        }
    }

    return 0;

}

void deinit_program() {

    dj_free_sfx(main_info, SFX_DEATH);
    dj_free_sfx(main_info, SFX_SPRING);
    dj_free_sfx(main_info, SFX_SPLASH);
    dj_deinit(main_info);

    if (background_pic != 0)
        free(background_pic);
    if (mask_pic != 0)
        free(mask_pic);


    if (!main_info.error_str.empty()) {
        printf("%s", main_info.error_str.c_str());
        exit(1);
    } else
        exit(0);

}


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


