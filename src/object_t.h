//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_T_H
#define JUMPNBUMP_OBJECT_T_H

struct object_t{
	int used, type;
	int x, y;
	int x_add, y_add;
	int x_acc, y_acc;
	int anim;
	int frame, ticks;
	int image;

	object_t(int type, int x, int y, int x_add, int y_add, int anim, int frame);

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