//
// Created by edwin on 13-06-18.
//

#include "game_manager_t.h"
#include "gob_t.h"
#include "leftovers_t.h"
#include "player_t.h"
#include "anim_t.h"
#include "globals.h"
#include "ban_map.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "objects_t.h"
#include "util.h"
#include "sound_manager_t.h"

game_manager_t::game_manager_t(sf::RenderWindow& window) :window(window) {
}

void game_manager_t::init_textures() {

    this->object_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/objects.png");
    this->background_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/level.png");
    this->foreground_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/mask.png");
    this->rabbit_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/rabbit.png");

}

void game_manager_t::init_deprecated_data() {
    player_anims = {
            {0, {{ 0, 0x7fff}}},
            {0, {{0, 4}, {1, 4}, {2, 4}, {3, 4}}},
            {0, {{4, 0x7fff}}},
            {2, {{5, 8}, {6, 10}, {7, 3}, {6, 3}}},
            {0, {{6, 0x7fff}}},
            {1, {{5, 8}, {4, 0x7fff}}},
            {0, {{8, 5}}}
    };



    rabbit_gobs.add("/home/edwin/Projects/jumpnbump/data/rabbit.json", this->rabbit_texture);


    object_gobs.add("/home/edwin/Projects/jumpnbump/data/objects.json", this->object_texture);


    if (!ban_map.read_from_file("/home/edwin/Projects/jumpnbump/data/levelmap.txt")) {
        printf("Error loading 'rabbit.gob', aborting...\n");
        return;
    }
}

void game_manager_t::draw() {
    this->window.clear();
    sf::Sprite background(this->background_texture);

    this->window.draw(background);


    for (int i = 0 ; i < players.size(); i++) {
        this->pobs.add(players[i].get_position(), players[i].anim_handler.image + i * 18, &rabbit_gobs);
    }

    this->pobs.draw(*this);
    leftovers.draw(*this);

    sf::Sprite foreground(this->foreground_texture);
    this->window.draw(foreground);

    this->window.display();

}

void game_manager_t::draw_pob(const pob_t& pob)
{
    auto& image = pob.pob_data->images[pob.image];
    image.setPosition(pob.position.x, pob.position.y);
    this->window.draw(image);

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

void game_manager_t::process_input() {
    sf::Event event;
    while (this->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            this->window.close();
        if ( event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased ) {
            if ( event.key.code == sf::Keyboard::Key::Escape ) {
                this->window.close();
            } else {
                for ( auto& player : players) {
                    player.control.update(event);
                }
            }
        }
    }
}

bool game_manager_t::init() {

        this->init_textures();
        this->init_deprecated_data();
        init_players(*this);

        for (auto& player : this->players) {
            player.reset_kills(*this);
            position_player(*this, player);
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


        return true;

}

void game_manager_t::loop() {


    this->sound_manager.load_sfx();
    this->sound_manager.load_music();
    this->sound_manager.play_music();

    this->init();
    printf("hola como te va");


    int update_count = 1;

    this->reset_frames();

    this->process_input();

    while (this->window.isOpen()) {
        while (update_count) {

            steer_players(*this);



            collision_check(*this);


            objects.update(*this);


            if (update_count == 1) {
                this->draw();




            }

            update_count--;
        }

        this->process_input();
        update_count = this->get_elapsed_frames();

    }
}