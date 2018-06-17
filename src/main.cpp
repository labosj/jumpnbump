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

#include "sound_manager_t.h"
#include "globals.h"
#include "gob_t.h"
#include "anim_t.h"
#include "leftovers_t.h"
#include "util.h"
#include "player_t.h"
#include "data.h"
#include "ban_map.h"
#include <string>
#include "object_t.h"
#include "anim_t.h"
#include "main_info.h"
#include "objects_t.h"
#include "sound_manager_t.h"
#include "game_manager_t.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "game_manager_t.h"
#include "application_t.h"

int pogostick, bunnies_in_space, jetpack;

static void game_loop(void) {


    external_sound_manager.reset(new sound_manager_t);

    external_sound_manager->load_sfx();
    external_sound_manager->load_music();
    external_sound_manager->play_music();

    external_game_manager->init();
    printf("hola como te va");



    bunnies_in_space = jetpack = pogostick = 0;

    int update_count = 1;
    int end_loop_flag = 0;

    external_game_manager->reset_frames();

    external_game_manager->process_input();

    while (external_game_manager->window.isOpen()) {
        while (update_count) {
/*
            if (endscore_reached || (key_pressed(1) == 1)) {
                end_loop_flag = 1;
            }
*/

            steer_players();



            collision_check();


            objects.update();


            if (update_count == 1) {
                external_game_manager->draw();




            }

            update_count--;
        }

        external_game_manager->process_input();
        update_count = external_game_manager->get_elapsed_frames();

    }
}


int main(int argc, char *argv[]) {


    application_t application;
    int result;
    if (application.init() ) {
        game_loop();

    }

    return result;

}



