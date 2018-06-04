//
// Created by edwin on 03-06-18.
//

#ifndef JUMPNBUMP_JOY_T_H
#define JUMPNBUMP_JOY_T_H

struct joy_t {
	int x, y;
	int raw_x, raw_y;
	int but1, but2;
	struct {
		int x1, x2, x3;
		int y1, y2, y3;
	} calib_data;
};
#endif //JUMPNBUMP_JOY_T_H
