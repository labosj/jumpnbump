//
// Created by edwin on 01-06-18.
//

#ifndef JUMPNBUMP_PLAYER_H
#define JUMPNBUMP_PLAYER_H

#include <vector>
#include "position_t.h"
#include "anim_handler_t.h"
#include "player_control_t.h"

class game_manager_t;

class player_t {
private:
	int id;
	game_manager_t& game_manager;

public:


	int action_left,action_up,action_right;
	int dead_flag;
	int bumps;
	//vector that stores the kill to other players
	std::vector<int> bumped;

	position_t position;

	int x_add, y_add;
	int direction, jump_ready, jump_abort, in_water;
	anim_handler_t anim_handler;

	player_control_t control;

	player_t(game_manager_t& game_manager, int id);

	int get_id() const { return this->id; }

	position_t get_position() const;

	bool is_alive() const { return this->dead_flag == 0; }

	void set_position(const position_t& position);
	void check_spring_jump();

	void reset_kills();
	void count_kill(const player_t& victim) {
		this->bumps++;
		this->bumped[victim.get_id()]++;
	}

	void gravity_fall();

	void check_ceiling();

	void check_lateral_walls();

	void set_anim(int anim);

	static void kill(game_manager_t &game_manager, int killer, int victim);

	void update_movement();
};

void check_collision(game_manager_t& game_manager, player_t &player_1, player_t &player_2);

void steer_players(game_manager_t &game_manager);

void position_player(game_manager_t& game_manager, player_t& player);

#endif //JUMPNBUMP_PLAYER_H
