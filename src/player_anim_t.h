//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_PLAYER_ANIM_T_H
#define JUMPNBUMP_PLAYER_ANIM_T_H

#include "anim_frame_t.h"
#include <vector>

struct player_anim_t {
	int restart_frame;
	std::vector<anim_frame_t> frame;
};
#endif //JUMPNBUMP_PLAYER_ANIM_T_H
