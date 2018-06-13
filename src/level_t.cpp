//
// Created by edwin on 12-06-18.
//

#include "level_t.h"
#include "globals.h"

level_t* external_level = nullptr;

void level_t::load_music() {
    this->music.openFromFile("/home/edwin/Projects/jumpnbump/data/bump.ogg");
    this->music.setLoop(true);
}

void level_t::load_sfx() {
    this->jump_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/jump.ogg"); //SFX_JUMP) != 0)

    this->death_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/death.ogg"); //SFX_DEATH) != 0)

    this->spring_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/spring.ogg"); //SFX_SPRING) != 0)

    this->splash_buffer.loadFromFile("/home/edwin/Projects/jumpnbump/data/splash.ogg"); //SFX_SPLASH) != 0) {
}

void level_t::play_music() {
    this->music.play();
}


void level_t::play_sfx(int index) {

    auto& buffer = ([this, index]() -> auto& {
        if ( index == SFX_JUMP ) {
            return this->jump_buffer;
        } else if ( index == SFX_DEATH ) {
            return this->death_buffer;
        } else if ( index == SFX_SPRING ) {
            return this->spring_buffer;
        } else {
            return this->splash_buffer;
        }
    })();

    for ( auto& sound : this->sounds ) {
        if ( sound.getStatus() == sf::Sound::Stopped ) {
            sound.setBuffer(buffer);
            sound.play();
            return;
        }
    }

    this->sounds.emplace_back(buffer);
    this->sounds.back().play();







}