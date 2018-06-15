//
// Created by edwin on 31-05-18.
//

#include "gob_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <nlohmann/json.hpp>
#include <fstream>

gob_t rabbit_gobs;
gob_t object_gobs;

int gob_t::add(const std::string& filename, sf::Texture& texture)
{

    std::ifstream file{filename};
    nlohmann::json json;
    file >> json;

    for (auto& element : json) {

        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(element["x"], element["y"], element["width"], element["height"]));
        sprite.setOrigin(element["hotspot_x"], element["hotspot_y"]);
        this->images.push_back(sprite);

    }
    return 0;

}