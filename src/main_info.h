//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_MAIN_INFO_H
#define JUMPNBUMP_MAIN_INFO_H

#include <string>

struct gob_t;

struct main_info_t {

	static const int NUM_POBS = 200;

	int joy_enabled;
	int no_sound;
	int music_no_sound;
	int no_gore;
	std::string error_str;
	int draw_page, view_page;
	struct {
		int num_pobs;
		struct {
			int x, y;
			int image;
			gob_t *pob_data;
			int back_buf_ofs;
		} pobs[NUM_POBS];
	} page_info[2];
	void *pob_backbuf[2];
};
#endif //JUMPNBUMP_MAIN_INFO_H
