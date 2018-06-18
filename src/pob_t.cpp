//
// Created by edwin on 05-06-18.
//

#include "position_t.h"
#include "pob_t.h"
#include "gob_t.h"

pob_t::pob_t(const screen_position_t& position, int img_number, gob_t* pob_data) {
    this->image = pob_data->images[img_number];
    this->image.setPosition(position.x, position.y);

}

sf::Sprite& pob_t::get_image() {
    return image;
}

