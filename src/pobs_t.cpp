//
// Created by edwin on 10-06-18.
//

#include <cstdlib>
#include "pobs_t.h"
#include "game_manager_t.h"

void pobs_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    this->pobs.emplace_back(position, image, pob_data);
}

void pobs_t::draw(game_manager_t& game_manager) {

    for (auto& pob : this->pobs) {

        game_manager.draw_pob(pob);
    }

    this->pobs.clear();

}
