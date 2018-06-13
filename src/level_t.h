//
// Created by edwin on 12-06-18.
//

#ifndef JUMPNBUMP_LEVEL_T_H
#define JUMPNBUMP_LEVEL_T_H

#include <SFML/Audio.hpp>

class level_t {
public:
    sf::SoundBuffer jump_buffer;
    sf::SoundBuffer death_buffer;
    sf::SoundBuffer spring_buffer;
    sf::SoundBuffer splash_buffer;

    sf::Music music;

    std::vector<sf::Sound> sounds;



    void load_music();
    void load_sfx();

    void play_music();
    void play_sfx(int index);
};

[[deprecated]]
extern level_t* external_level;


#endif //JUMPNBUMP_LEVEL_T_H
