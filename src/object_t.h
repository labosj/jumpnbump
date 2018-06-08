//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_T_H
#define JUMPNBUMP_OBJECT_T_H

class player_t;

#include "position_t.h"
#include "anim_t.h"

void add_smoke(const player_t& player);
void add_jetpack_smoke(const player_t& player);
void add_object(int type, const position_t& position, int x_add, int y_add, int anim, int frame);

struct object_t{
	int used, type;

	position_t position;

	int x_add, y_add;
	int x_acc, y_acc;
	int anim;
	int frame, ticks;
	int image;

	object_t(int type, const position_t& position, int x_add, int y_add, int anim, int frame);


	/**
	 * Read only
	 * @return
	 */
	position_t get_position() const {
		return this->position;
	}

	void update_flesh_trace();
	void update_smoke();
	void update_spring();
	void update_splash();
	void update_butterfly();
	void update_flesh();
	void update_fur();

	/**
	 * Is this element an active element?
	 * @return
	 */
	bool is_used() const { return this->used == 1; }

	/**
	 * Is this element available to create a new element?
	 * @return
	 */
	bool is_available() const { return this->used == 0; }

	/**
	 * Set this element available.
	 *
	 * Use this function when you want to free this element as a new element
	 */
	void set_available() { this->used = 0; }
};

#endif //JUMPNBUMP_OBJECT_T_H
