//
// Created by edwin on 16-06-18.
//

#ifndef JUMPNBUMP_PLAYER_CONTROL_T_H
#define JUMPNBUMP_PLAYER_CONTROL_T_H

#include <SFML/Window/Event.hpp>

class player_control_t {

public:

    bool up = false;
    bool left = false;
    bool right = false;

    sf::Keyboard::Key up_key =  sf::Keyboard::Key::Up;
    sf::Keyboard::Key left_key = sf::Keyboard::Key::Left;
    sf::Keyboard::Key right_key = sf::Keyboard::Key::Right;

    void update(const sf::Event& event) {
        if ( event.key.code ==  this->left_key) {
            this->left = this->toBool(event);
        } else if ( event.key.code ==  this->right_key) {
            this->right = this->toBool(event);

        } else if ( event.key.code == this->up_key ) {
            this->up = this->toBool(event);
        }

    }

    bool toBool(const sf::Event& event) const {
        return !(event.type == sf::Event::KeyReleased);
    }

    bool up_pressed() const {
        return this->up;
    }

    bool left_pressed() const {
        return this->left;
    }

    bool right_pressed() const {
        return this->right;
    }
};


#endif //JUMPNBUMP_PLAYER_CONTROL_T_H
