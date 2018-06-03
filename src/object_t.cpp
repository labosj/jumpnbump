//
// Created by edwin on 02-06-18.
//

#include "object_t.h"
#include "object_anim.h"

extern object_anim_t object_anims[8];

object_t::object_t(int type, int x, int y, int x_add, int y_add, int anim, int frame) {
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

void object_t::update_flesh_trace() {
    this->ticks--;
    if (this->ticks <= 0) {
        this->frame++;
        if (this->frame >= object_anims[this->anim].num_frames)
            this->used = 0;
        else {
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    }

}

void object_t::update_smoke() {
    this->x += this->x_add;
    this->y += this->y_add;
    this->ticks--;
    if (this->ticks <= 0) {
        this->frame++;
        if (this->frame >= object_anims[this->anim].num_frames)
            this->used = 0;
        else {
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    }
}

void object_t::update_spring() {
    this->ticks--;
    if (this->ticks <= 0) {
        this->frame++;
        if (this->frame >= object_anims[this->anim].num_frames) {
            this->frame--;
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
        } else {
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    }
}

void object_t::update_splash() {
    this->ticks--;
    if (this->ticks <= 0) {
        this->frame++;
        if (this->frame >= object_anims[this->anim].num_frames)
            this->used = 0;
        else {
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    }
}