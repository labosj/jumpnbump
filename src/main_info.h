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
	int no_gore;
	std::string error_str;
	int draw_page, view_page;

	struct {
		std::vector<pob_t> pobs;
	} page_info[2];

	void *pob_backbuf[2];
};
#endif //JUMPNBUMP_MAIN_INFO_H
