//
// Created by edwin on 17-06-18.
//

#ifndef JUMPNBUMP_STAGE_T_H
#define JUMPNBUMP_STAGE_T_H


#include <SFML/Graphics/Texture.hpp>
#include <SFML/Audio/Music.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "ban_map_t.h"
#include "position_component_t.h"

class stage_t {

private:
    sf::Texture background_texture;
    sf::Texture foreground_texture;

    sf::Sprite background_sprite;
    sf::Sprite foreground_sprite;

    sf::Music background_music;

    ban_map_t ban_map;

    position_component_t gravity{32768};

public:
    void load(const std::string& folder);

    void draw_background(sf::RenderTarget& target);
    void draw_foreground(sf::RenderTarget& target);

    void play_bgm();

    ban_map_t& get_map();

};


#endif //JUMPNBUMP_STAGE_T_H
