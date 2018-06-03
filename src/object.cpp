//
// Created by edwin on 02-06-18.
//

#include "object.h"
#include "object_anim.h"

extern object_anim_t object_anims[8];

object::object(int type, int x, int y, int x_add, int y_add, int anim, int frame) {
    this->used = 1;
    this->type = type;
    this->x = (long) x << 16;
    this->y = (long) y << 16;
    this->x_add = x_add;
    this->y_add = y_add;
    this->x_acc = 0;
    this->y_acc = 0;
    this->anim = anim;
    this->frame = frame;
    this->ticks = object_anims[anim].frame[frame].ticks;
    this->image = object_anims[anim].frame[frame].image;
}