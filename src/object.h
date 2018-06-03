//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_T_H
#define JUMPNBUMP_OBJECT_T_H

struct object{
	int used, type;
	int x, y;
	int x_add, y_add;
	int x_acc, y_acc;
	int anim;
	int frame, ticks;
	int image;

	object(int type, int x, int y, int x_add, int y_add, int anim, int frame);
};
#endif //JUMPNBUMP_OBJECT_T_H
