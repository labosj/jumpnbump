//
// Created by edwin on 31-05-18.
//

#include "main_info.h"
#include "dj.h"

main_info_t main_info;

main_info_t::~main_info_t() {
/*
    dj_free_sfx(*this, SFX_DEATH);
    dj_free_sfx(*this, SFX_SPRING);
    dj_free_sfx(*this, SFX_SPLASH);
    dj_deinit(*this);
*/

    if (!main_info.error_str.empty()) {
        printf("%s", main_info.error_str.c_str());
        exit(1);
    } else
        exit(0);
}