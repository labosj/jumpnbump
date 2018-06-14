//
// Created by edwin on 13-06-18.
//

#include "game_manager_t.h"
#include "gob_t.h"
#include "leftovers_t.h"
#include "player_t.h"
#include "main_info.h"
#include <SFML/Window.hpp>

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

void game_manager_t::draw() {
    this->window.clear();

    //window.draw(shape);


    for (int i = 0 ; i < players.size(); i++) {
        main_info.pobs.add(players[i].get_position(), players[i].anim_handler.image + i * 18, &rabbit_gobs);
    }

    main_info.pobs.draw();
    leftovers.draw();


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