//
// Created by edwin on 10-06-18.
//

#ifndef JUMPNBUMP_POBS_T_H
#define JUMPNBUMP_POBS_T_H

#include <vector>
#include "pob_t.h"

class game_manager_t;


class pobs_t {
private:
    std::vector<pob_t> pobs;
public:


    void add(const screen_position_t &position, int image, gob_t *pob_data);

    void draw(game_manager_t& game_manager);
};


#endif //JUMPNBUMP_POBS_T_H
