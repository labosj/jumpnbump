//
// Created by edwin on 20-09-18.
//

#ifndef JUMPNBUMP_CHARACTER_T_H
#define JUMPNBUMP_CHARACTER_T_H

#include <SFML/Graphics/Texture.hpp>
#include "gob_t.h"
#include "anim_t.h"

class character_t {

public:
    void load(const std::string& folder);

    std::string name;
    sf::Texture texture;
    gob_t gobs;
    std::vector<anim_t> anims;
};


#endif //JUMPNBUMP_CHARACTER_T_H
