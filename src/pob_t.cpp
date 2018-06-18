//
// Created by edwin on 05-06-18.
//

#include "position_t.h"
#include "pob_t.h"
#include "gob_t.h"

sf::Sprite& pob_t::get_image() {
    auto& image = this->pob_data->images[this->image];
    image.setPosition(this->position.x, this->position.y);
    return image;
}

