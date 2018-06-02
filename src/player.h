//
// Created by edwin on 01-06-18.
//

#ifndef JUMPNBUMP_PLAYER_H
#define JUMPNBUMP_PLAYER_H

#include <src/globals.h>

const unsigned int JNB_MAX_PLAYERS = 4;

typedef struct {
	int action_left,action_up,action_right;
	int enabled, dead_flag;
	int bumps;
	int bumped[JNB_MAX_PLAYERS];
	int x, y;
	int x_add, y_add;
	int direction, jump_ready, jump_abort, in_water;
	int anim, frame, frame_tick, image;
} player_t;
#endif //JUMPNBUMP_PLAYER_H
