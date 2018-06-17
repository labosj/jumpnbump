//
// Created by edwin on 13-06-18.
//

#ifndef JUMPNBUMP_GAME_MANAGER_T_H
#define JUMPNBUMP_GAME_MANAGER_T_H

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include "sound_manager_t.h"
#include <memory>

class pob_t;

class game_manager_t {
public:
    sf::Clock clock;
    int frame_counter = 0;
    sf::RenderWindow& window;

    sf::Texture object_texture;
    sf::Texture rabbit_texture;
    sf::Texture background_texture;
    sf::Texture foreground_texture;

    sound_manager_t sound_manager;

    game_manager_t(sf::RenderWindow& window);

    void init_textures();

    void init_deprecated_data();


    void draw();
    void draw_pob(const pob_t& pob);
    void reset_frames();
    int get_elapsed_frames();

    void process_input();

    bool init();

    void loop();
};


#endif //JUMPNBUMP_GAME_MANAGER_T_H
