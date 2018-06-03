//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_PLAYER_ANIM_T_H
#define JUMPNBUMP_PLAYER_ANIM_T_H

struct player_anim_t {
	int num_frames;
	int restart_frame;
	struct {
		int image;
		int ticks;
	} frame[4];
};
#endif //JUMPNBUMP_PLAYER_ANIM_T_H
