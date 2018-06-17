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

    void update(const sf::Event& event) {
        if ( event.key.code == sf::Keyboard::Key::Left) {
            this->left = this->toBool(event);
        } else if ( event.key.code == sf::Keyboard::Key::Right) {
            this->right = this->toBool(event);

        } else if ( event.key.code == sf::Keyboard::Key::Up ) {
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
