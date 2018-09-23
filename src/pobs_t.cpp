//
// Created by edwin on 10-06-18.
//

#include <cstdlib>
#include "pobs_t.h"
#include "game_manager_t.h"
#include "gob_t.h"

void pobs_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    sf::Sprite sprite = pob_data->get_sprite(image);
    sprite.setPosition(position.x.value, position.y.value);

    this->pobs.push_back(sprite);
}

void pobs_t::add(const sf::Sprite& sprite) {
    this->pobs.push_back(sprite);
}

void pobs_t::draw(game_manager_t& game_manager) {

    for (auto& pob : this->pobs) {

        game_manager.window.draw(pob);
    }

}

void pobs_t::clear() {
    this->pobs.clear();
}
