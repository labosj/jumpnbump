//
// Created by edwin on 13-06-18.
//

#ifndef JUMPNBUMP_GAME_MANAGER_T_H
#define JUMPNBUMP_GAME_MANAGER_T_H

#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>


class game_manager_t {
public:
    sf::Clock clock;
    int frame_counter = 0;
    sf::RenderWindow window;

    sf::Texture object_texture;
    sf::Texture rabbit_texture;
    sf::Texture background_texture;
    sf::Texture foreground_texture;

    void init_window();

    void init_textures();

    void init_deprecated_data();


    void draw();
    void reset_frames();
    int get_elapsed_frames();
};

extern std::unique_ptr<game_manager_t> external_game_manager;

#endif //JUMPNBUMP_GAME_MANAGER_T_H
