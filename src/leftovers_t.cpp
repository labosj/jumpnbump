//
// Created by edwin on 02-06-18.
//

#include "gob_t.h"
#include "leftovers_t.h"
#include "screen_position_t.h"
#include "game_manager_t.h"

void leftovers_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    this->pobs.emplace_back(position, image, pob_data);
}

void leftovers_t::draw(game_manager_t& game_manager) {
    for (auto& pob : this->pobs)
        game_manager.draw_pob(pob);

}