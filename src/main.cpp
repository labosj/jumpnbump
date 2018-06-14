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
#include "sdl/gfx.h"
#include "object_t.h"
#include "anim_t.h"
#include "joy_t.h"
#include "main_info.h"
#include "objects_t.h"
#include "sound_manager_t.h"
#include "game_manager_t.h"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "game_manager_t.h"

#ifndef M_PI
#define M_PI		3.14159265358979323846
#endif
int endscore_reached;

joy_t joy;

int pogostick, bunnies_in_space, jetpack;

void serverSendKillPacket(int killer, int victim) {
    int c1 = killer;
    int c2 = victim;
    int c4 = 0;

    players[c1].y_add = -players[c1].y_add;
    if (players[c1].y_add > -262144L)
        players[c1].y_add = -262144L;
    players[c1].jump_abort = 1;
    players[c2].dead_flag = 1;
    if (players[c2].anim_handler.anim != 6) {
        players[c2].set_anim(6);
        if (main_info.gore ) {
            auto screen_position = screen_position_t{players[victim].get_position()} + screen_position_t{6 + rnd(5), 6 + rnd(5)};
            for (c4 = 0; c4 < 6; c4++)
                objects.add(object_t::Type::FUR, screen_position, (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 44 + c2 * 8);
            for (c4 = 0; c4 < 6; c4++)
                objects.add(object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 76);
            for (c4 = 0; c4 < 6; c4++)
                objects.add(object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 77);
            for (c4 = 0; c4 < 8; c4++)
                objects.add(object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 78);
            for (c4 = 0; c4 < 10; c4++)
                objects.add(object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                           (rnd(65535) - 32768) * 3, 0, 79);
        }
        external_sound_manager->play_sfx_death();


        players[c1].count_kill(c2);
        if (players[c1].bumps >= JNB_END_SCORE) {
            endscore_reached = 1;
        }
    }
}

static void game_loop(void) {

    int update_count = 1;
    int end_loop_flag = 0;

    external_game_manager->reset_frames();

    intr_sysupdate();

    endscore_reached = 0;

    //set_blood_is_thicker_than_water();
    while (external_game_manager->window.isOpen()) {
        while (update_count) {

            if (endscore_reached || (key_pressed(1) == 1)) {
                end_loop_flag = 1;
            }


            steer_players();



            collision_check();


            objects.update();


            if (update_count == 1) {
                external_game_manager->draw();




            }

            update_count--;
        }

        intr_sysupdate();
        update_count = external_game_manager->get_elapsed_frames();


        if (end_loop_flag == 1)
            break;
    }
}


static int menu_loop() {


    external_sound_manager.reset(new sound_manager_t);

    external_sound_manager->load_sfx();
    external_sound_manager->load_music();
    external_sound_manager->play_music();

        init_level();
        printf("hola como te va");



        bunnies_in_space = jetpack = pogostick = 0;
        //blood_is_thicker_than_water = 1; HERE IS TO MOD THE CHEATS

        game_loop();


}


int main(int argc, char *argv[]) {

//    "/home/edwin/Projects/jumpnbump/data/level.pcx"
/*
    sf::RenderWindow window(sf::VideoMode(200, 200), "Jump N Bump");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }

    return 0;
    */


    int result;
    printf("holahola");
    if (init_program(argc, argv) == 0) {
        printf("holahola");
        result = menu_loop();

    }

    return result;

}

int init_level() {

    external_game_manager->init_window();
    external_game_manager->init_textures();
    external_game_manager->init_deprecated_data();
    init_players();

    for (auto& player : players) {
            player.reset_kills();
            position_player(player);
    }

    for (int c1 = 0; c1 < ban_map.get_height(); c1++) {
        for (int c2 = 0; c2 < ban_map.get_width() ; c2++) {
            if (ban_map.get(map_position_t{c2, c1}) == ban_map_t::Type::SPRING)
                objects.add(object_t::Type::SPRING, map_position_t{c2, c1}, 0, 0, OBJ_ANIM_SPRING, 5);
        }
    }

    for ( int i = 0 ; i < 2 ; i++ ) {
        auto new_pos = ban_map.get_random_available_position();
        objects.add(object_t::Type::YEL_BUTFLY, screen_position_t{8, 8} + new_pos , (rnd(65535) - 32768) * 2,
                   (rnd(65535) - 32768) * 2,
                   0, 0);
    }

    for ( int i = 0 ; i < 2 ; i++ ) {
        auto new_pos = ban_map.get_random_available_position();
        objects.add(object_t::Type::PINK_BUTFLY,  screen_position_t{8, 8} + new_pos, (rnd(65535) - 32768) * 2,
                   (rnd(65535) - 32768) * 2, 0, 0);
    }


    return 0;

}

#ifndef PATH_MAX
#define PATH_MAX 1024
#endif

int init_program(int argc, char *argv[]) {


    external_game_manager.reset(new game_manager_t);

    srand(time(NULL));

    init_inputs(main_info);

    return 0;

}



