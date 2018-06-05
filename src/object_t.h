//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_T_H
#define JUMPNBUMP_OBJECT_T_H

#include "position_t.h"

struct object_t{
	int used, type;

	int x;

	int y;

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
		return position_t{this->x, this->y};
	}

	void update_flesh_trace();
	void update_smoke();
	void update_spring();
	void update_splash();
	void update_butterfly();
	void update_flesh();
	void update_fur();

	bool is_used() const { return this->used == 1; }
};
#endif //JUMPNBUMP_OBJECT_T_H
