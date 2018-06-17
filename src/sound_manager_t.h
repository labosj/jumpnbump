//
// Created by edwin on 12-06-18.
//

#ifndef JUMPNBUMP_LEVEL_T_H
#define JUMPNBUMP_LEVEL_T_H

#include <SFML/Audio.hpp>

class sound_manager_t {
public:
    sf::SoundBuffer jump_buffer;
    sf::SoundBuffer death_buffer;
    sf::SoundBuffer spring_buffer;
    sf::SoundBuffer splash_buffer;

    std::vector<sf::Sound> sounds;



    void load_sfx();

    void play_sfx(const sf::Sound& new_sound);
    void play_sfx_spring();
    void play_sfx_jump();
    void play_sfx_death();
    void play_sfx_splash();
};

#endif //JUMPNBUMP_LEVEL_T_H
