//
// Created by edwin on 17-06-18.
//

#include "stage_t.h"
#include <SFML/Graphics/RenderTarget.hpp>

void stage_t::load(const std::string& folder) {
    this->background_texture.loadFromFile(folder + "/background.png");
    this->foreground_texture.loadFromFile(folder + "/foreground.png");

    this->background_sprite.setTexture(this->background_texture);
    this->foreground_sprite.setTexture(this->foreground_texture);

    this->background_music.openFromFile(folder + "/bgm.ogg");
    this->background_music.setLoop(true);

    if (!this->ban_map.read_from_file(folder +  "/map.txt")) {
        printf("Error loading 'rabbit.gob', aborting...\n");
        return;
    }
}

ban_map_t& stage_t::get_map() {
    return this->ban_map;
}

void stage_t::draw_background(sf::RenderTarget& target) {
    target.draw(this->background_sprite);
}

void stage_t::draw_foreground(sf::RenderTarget& target) {
    target.draw(this->foreground_sprite);
}

void stage_t::play_bgm() {
    this->background_music.play();
}

