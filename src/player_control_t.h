//
// Created by edwin on 16-06-18.
//

#ifndef JUMPNBUMP_PLAYER_CONTROL_T_H
#define JUMPNBUMP_PLAYER_CONTROL_T_H


class player_control_t {

public:

    bool up = false;
    bool down = false;
    bool right = false;

    void update(const sf::Event& event) {
        if ( event.key.code == sf::Event::KeyEvent::key:)
        if (event.type == sf::Event::KeyReleased)
            addkey(event.key.code | 0x8000);
        else {
            addkey(event.key.code | 0x7fff);
        }
    }

    bool toBool(sf::Event& event) {
        if ( event.type == sf::Event::KeyReleased)
            return false;
        else
            return true;
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
