//
// Created by edwin on 13-06-18.
//

#include "game_manager_t.h"
#include "gob_t.h"
#include "leftovers_t.h"
#include "player_t.h"
#include "main_info.h"
#include "anim_t.h"
#include "data.h"
#include "globals.h"
#include "ban_map.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Sprite.hpp>

std::unique_ptr<game_manager_t> external_game_manager = nullptr;

void game_manager_t::init_window() {
    this->window.create(sf::VideoMode(400, 256), "My window");
}

void game_manager_t::init_textures() {

    this->object_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/objects.png");
    this->background_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/level.png");
    this->foreground_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/mask.png");
    this->rabbit_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/rabbit.png");

}

void game_manager_t::init_deprecated_data() {
    std::string datfile_name = "/home/edwin/Projects/jumpnbump/data/jumpbump.dat";

    unsigned char *datafile_buffer = nullptr;

    preread_datafile(datfile_name, &datafile_buffer);

    player_anims = {
            {0, {{ 0, 0x7fff}}},
            {0, {{0, 4}, {1, 4}, {2, 4}, {3, 4}}},
            {0, {{4, 0x7fff}}},
            {2, {{5, 8}, {6, 10}, {7, 3}, {6, 3}}},
            {0, {{6, 0x7fff}}},
            {1, {{5, 8}, {4, 0x7fff}}},
            {0, {{8, 5}}}
    };

    unsigned char *handle = nullptr;



    if ((handle = dat_open("rabbit.gob", datafile_buffer)) == nullptr) {
        printf("Error loading 'rabbit.gob1', aborting...\n");
        return;
    }
    if (rabbit_gobs.add(handle, dat_filelen("rabbit.gob", datafile_buffer), this->rabbit_texture)) {
        printf("Error loading 'rabbit.gob2', aborting...\n");
        return;
    }

    if ((handle = dat_open("objects.gob", datafile_buffer)) == nullptr) {
        printf("Error loading 'rabbit.gob3', aborting...\n");
        return;
    }
    if (object_gobs.add(handle, dat_filelen("objects.gob", datafile_buffer), this->object_texture)) {
        /* error */
        return;
    }

    if (!ban_map.read_from_file("/home/edwin/Projects/jumpnbump/data/levelmap.txt")) {
        printf("Error loading 'rabbit.gob', aborting...\n");
        return;
    }
    if (main_info.joy_enabled == 1) {
        if ((handle = dat_open("calib.dat", datafile_buffer)) == 0) {
            printf("Error loading 'rabbit.gob', aborting...\n");
            return;
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
    }
}

void game_manager_t::draw() {
    this->window.clear();
    sf::Sprite background(this->background_texture);

    this->window.draw(background);


    for (int i = 0 ; i < players.size(); i++) {
        main_info.pobs.add(players[i].get_position(), players[i].anim_handler.image + i * 18, &rabbit_gobs);
    }

    main_info.pobs.draw();
    leftovers.draw();

    sf::Sprite foreground(this->foreground_texture);
    this->window.draw(foreground);

    this->window.display();

}

void game_manager_t::reset_frames() {
    this->clock.restart();
    this->frame_counter= 0;
}

int game_manager_t::get_elapsed_frames() {
    auto elapsed = this->clock.getElapsedTime();

    const auto milliseconds_per_frame = 1000 / 60;

    auto frames = elapsed.asMilliseconds() / milliseconds_per_frame;;
    if ( this->frame_counter < frames ) {
        auto elapsed_frames = frames - this->frame_counter;
        this->frame_counter = frames;
        return elapsed_frames;
    } else {
        return 0;
    }



}