//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_MAIN_INFO_H
#define JUMPNBUMP_MAIN_INFO_H

#include <string>
#include <vector>
#include "pob_t.h"

struct gob_t;

struct main_info_t {

	int joy_enabled;
	int no_sound = 0;
	int music_no_sound = 0;
	bool gore = true;
	std::string error_str;
	int draw_page, view_page;

	std::vector<pob_t> pobs;

	void *pob_backbuf;
};

extern main_info_t main_info;
#endif //JUMPNBUMP_MAIN_INFO_H
