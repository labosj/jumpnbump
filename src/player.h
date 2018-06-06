//
// Created by edwin on 01-06-18.
//

#ifndef JUMPNBUMP_PLAYER_H
#define JUMPNBUMP_PLAYER_H

#include <vector>
#include "position_t.h"

const unsigned int JNB_MAX_PLAYERS = 4;

struct player_t {
	int action_left,action_up,action_right;
	int enabled, dead_flag;
	int bumps;
	//vector that stores the kill to other players
	std::vector<int> bumped;

	position_t position;

	int x_add, y_add;
	int direction, jump_ready, jump_abort, in_water;
	int anim, frame, frame_tick, image;

	position_t get_position() const;

	bool is_alive() const { return this->dead_flag == 0; }

	void set_position(const position_t& position);

	void reset_kills() {
		this->bumps = 0;
		this->bumped = std::vector<int>(JNB_MAX_PLAYERS, 0);
	}
	void count_kill(int victim) {
		this->bumps++;
		this->bumped[victim]++;
	}

};

void steer_players();

void position_player(int player_num);

void collision_check();

#endif //JUMPNBUMP_PLAYER_H
