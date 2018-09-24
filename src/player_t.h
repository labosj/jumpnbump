//
// Created by edwin on 01-06-18.
//

#ifndef JUMPNBUMP_PLAYER_H
#define JUMPNBUMP_PLAYER_H

#include <vector>
#include "position_t.h"
#include "anim_handler_t.h"
#include "player_control_t.h"
#include "bounding_box_t.h"

class game_manager_t;

class player_t {
private:
	int id;
	game_manager_t& game_manager;

public:

	enum class PLAYER_DIRECTION { LEFT, RIGHT };


	int character_id = 0;
	int action_left,action_up,action_right;
	bool dead_flag;
	bool pogostick = false;
	bool jetpack = false;
	int bumps;
	std::string player_name;
	//vector that stores the kill to other players
	std::vector<int> bumped;

	position_t position;

	int x_add, y_add;
	bool jump_ready;
	bool jump_abort;
	bool in_water;
	PLAYER_DIRECTION direction;
	anim_handler_t anim_handler;

	player_control_t control;

	player_t(game_manager_t& game_manager, int id);

	int get_id() const { return this->id; }
	game_manager_t& get_game_manager();

	position_t get_position() const;

	bool is_alive() const { return !this->dead_flag; }

	void set_position(const position_t& position);
	int get_character_id() const { return this->character_id; }
	void check_spring_jump();

	void reset_kills();
	void count_kill(const player_t& victim) {
		this->bumps++;
		this->bumped[victim.get_id()]++;
	}

	void gravity_fall();
	void animate();

	bounding_box_t get_bounding_box() const { return bounding_box_t{this->position, 12 << 16, 12 << 16};}

	bool collide(const player_t& player) const {
		return this->get_bounding_box().collide(player.get_bounding_box());
	}

	/**
	 * Use in conjuntion with collide
	 * @param player
	 * @return
	 */
	bool is_over(const player_t &player) const {

		auto stomper_bottom = this->get_bounding_box().get_bottom();

		auto stomped_top = player.get_bounding_box().get_top() + (5 << 16);

		return stomper_bottom < stomped_top;
	}

	/**
	 * Jumping of moving up.
	 *
	 * When is moving up it can't stomp another rabbits
	 * @return
	 */
	bool is_moving_up() const {
		return this->y_add < 0;
	}

	void bounce() {
		this->y_add = -this->y_add;
		if (this->y_add > -262144L)
			this->y_add = -262144L;
		this->jump_abort = true;
	}

	void check_ceiling();

	void check_lateral_walls();

	void set_anim(int anim);

	void update_movement();

	void position_player() ;

	static void check_collision(player_t &player_1, player_t &player_2);
	static void player_kill(player_t &player_1, player_t &player_2);

	void do_action_left();
	void do_action_right();
	void do_no_action();
};


#endif //JUMPNBUMP_PLAYER_H
