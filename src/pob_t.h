//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_POB_T_H
#define JUMPNBUMP_POB_T_H

#include <SFML/Graphics/Sprite.hpp>
#include "screen_position_t.h"

struct gob_t;

/**
 * Maybe this stands for player object
 */
struct pob_t {
private:
    screen_position_t position;

    int image;
    gob_t *pob_data;

public:
    sf::Sprite& get_image();


    pob_t(const screen_position_t& position, int image, gob_t* pob_data) {
        this->position = position;
        this->image = image;
        this->pob_data = pob_data;
    }
};

#endif //JUMPNBUMP_POB_T_H
