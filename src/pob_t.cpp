//
// Created by edwin on 05-06-18.
//

#include "position_t.h"
#include "pob_t.h"
#include "main_info.h"

extern main_info_t main_info;

int add_pob(int page, const screen_position_t& position, int image, gob_t* pob_data) {
    main_info.page_info[page].pobs.emplace_back(position, image, pob_data);

    return 0;
}


