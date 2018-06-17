//
// Created by edwin on 12-06-18.
//

#include "sound_manager_t.h"
#include "globals.h"
#include "util.h"

void sound_manager_t::load_sfx() {
    this->jump_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/jump.ogg");

    this->death_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/death.ogg");

    this->spring_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/spring.ogg");

    this->splash_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/splash.ogg");
}


void sound_manager_t::play_sfx_spring() {
    auto sound = sf::Sound{this->spring_buffer};
    //less scale make sound more grave
    auto scale = 0.9f;
    sound.setPitch( scale * (0.9f + rnd(100) * 0.002f));
    this->play_sfx(sound);
}

void sound_manager_t::play_sfx_jump() {
    auto sound = sf::Sound{this->jump_buffer};
    sound.setPitch( 0.9f + rnd(100) * 0.002f);
    this->play_sfx(sound);
}

void sound_manager_t::play_sfx_death() {
    auto sound = sf::Sound{this->death_buffer};
    sound.setPitch( 0.9f + rnd(100) * 0.002f);
    this->play_sfx(sound);
}

void sound_manager_t::play_sfx_splash() {
    auto sound = sf::Sound{this->splash_buffer};
    //less scale make sound more grave
    auto scale = 0.5f;
    sound.setPitch( scale *( 0.9f + rnd(100) * 0.002f));
    this->play_sfx(sound);
}

void sound_manager_t::play_sfx(const sf::Sound& new_sound) {
    for ( auto& sound : this->sounds ) {
        if ( sound.getStatus() == sf::Sound::Stopped ) {
            sound = new_sound;
            sound.play();
            return;
        }
    }

    this->sounds.emplace_back(new_sound);
    this->sounds.back().play();
}
