//
// Created by edwin on 31-05-18.
//

#include "main_info.h"

main_info_t main_info;

main_info_t::~main_info_t() {

    if (!main_info.error_str.empty()) {
        printf("%s", main_info.error_str.c_str());
        exit(1);
    } else
        exit(0);
}