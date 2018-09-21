//
// Created by edwin on 20-09-18.
//

#include "character_t.h"

void character_t::load(const std::string& folder) {
    this->texture.loadFromFile(folder + "/sprites.png");
    this->gobs.add_character(folder + "/data.json", this->texture);
}