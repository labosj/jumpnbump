//
// Created by edwin on 05-06-18.
//

#include "position_t.h"
#include "pob_t.h"
#include "gob_t.h"

sf::Sprite& pob_t::get_image() {
    return this->pob_data->images[this->image];
}

