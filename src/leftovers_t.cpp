//
// Created by edwin on 02-06-18.
//

#include "gob_t.h"
#include "globals.h"
#include "leftovers_t.h"
#include "screen_position_t.h"
#include "game_manager_t.h"

leftovers_t leftovers;

void leftovers_t::add(const screen_position_t &position, int image, gob_t *pob_data) {
    this->pobs.emplace_back(position, image, pob_data);
}

void leftovers_t::draw() {
    for (const auto& pob : this->pobs)
        external_game_manager->draw_pob(pob);

}