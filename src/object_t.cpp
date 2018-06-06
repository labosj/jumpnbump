//
// Created by edwin on 02-06-18.
//

#include "object_t.h"
#include "object_anim.h"
#include "ban_map.h"
#include "util.h"
#include "globals.h"
#include "draw.h"
#include "gob.h"

extern leftovers_t leftovers;
extern object_anim_t object_anims[8];

object_t::object_t(int type, const screen_position_t& position, int x_add, int y_add, int anim, int frame) {
    this->used = 1;
    this->type = type;

    this->x = (long) position.x << 16;
    this->y = (long) position.y << 16;
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

void object_t::update_butterfly() {
    this->x_acc += rnd(128) - 64;
    if (this->x_acc < -1024)
        this->x_acc = -1024;
    if (this->x_acc > 1024)
        this->x_acc = 1024;
    this->x_add += this->x_acc;
    if (this->x_add < -32768)
        this->x_add = -32768;
    if (this->x_add > 32768)
        this->x_add = 32768;
    this->x += this->x_add;
    if ((this->x >> 16) < 16) {
        this->x = 16 << 16;
        this->x_add = -this->x_add >> 2;
        this->x_acc = 0;
    } else if ((this->x >> 16) > 350) {
        this->x = 350 << 16;
        this->x_add = -this->x_add >> 2;
        this->x_acc = 0;
    }
    if (ban_map.get(this->get_position()) != 0) {
        if (this->x_add < 0) {
            this->x = (((this->x >> 16) + 16) & 0xfff0) << 16;
        } else {
            this->x = ((((this->x >> 16) - 16) & 0xfff0) + 15) << 16;
        }
        this->x_add = -this->x_add >> 2;
        this->x_acc = 0;
    }
    this->y_acc += rnd(64) - 32;
    if (this->y_acc < -1024)
        this->y_acc = -1024;
    if (this->y_acc > 1024)
        this->y_acc = 1024;
    this->y_add += this->y_acc;
    if (this->y_add < -32768)
        this->y_add = -32768;
    if (this->y_add > 32768)
        this->y_add = 32768;
    this->y += this->y_add;
    if ((this->y >> 16) < 0) {
        this->y = 0;
        this->y_add = -this->y_add >> 2;
        this->y_acc = 0;
    } else if ((this->y >> 16) > 255) {
        this->y = 255 << 16;
        this->y_add = -this->y_add >> 2;
        this->y_acc = 0;
    }
    if (ban_map.get(this->get_position()) != 0) {
        if (this->y_add < 0) {
            this->y = (((this->y >> 16) + 16) & 0xfff0) << 16;
        } else {
            this->y = ((((this->y >> 16) - 16) & 0xfff0) + 15) << 16;
        }
        this->y_add = -this->y_add >> 2;
        this->y_acc = 0;
    }
    if (this->type == OBJ_YEL_BUTFLY) {
        if (this->x_add < 0 && this->anim != OBJ_ANIM_YEL_BUTFLY_LEFT) {
            this->anim = OBJ_ANIM_YEL_BUTFLY_LEFT;
            this->frame = 0;
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        } else if (this->x_add > 0 && this->anim != OBJ_ANIM_YEL_BUTFLY_RIGHT) {
            this->anim = OBJ_ANIM_YEL_BUTFLY_RIGHT;
            this->frame = 0;
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    } else {
        if (this->x_add < 0 && this->anim != OBJ_ANIM_PINK_BUTFLY_LEFT) {
            this->anim = OBJ_ANIM_PINK_BUTFLY_LEFT;
            this->frame = 0;
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        } else if (this->x_add > 0 && this->anim != OBJ_ANIM_PINK_BUTFLY_RIGHT) {
            this->anim = OBJ_ANIM_PINK_BUTFLY_RIGHT;
            this->frame = 0;
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    }
    this->ticks--;
    if (this->ticks <= 0) {
        this->frame++;
        if (this->frame >= object_anims[this->anim].num_frames)
            this->frame = object_anims[this->anim].restart_frame;
        else {
            this->ticks = object_anims[this->anim].frame[this->frame].ticks;
            this->image = object_anims[this->anim].frame[this->frame].image;
        }
    }
}


void object_t::update_flesh() {
    if (rnd(100) < 30) {
        if (this->frame == 76)
            add_object(OBJ_FLESH_TRACE, this->get_position(), 0, 0,
                       OBJ_ANIM_FLESH_TRACE, 1);
        else if (this->frame == 77)
            add_object(OBJ_FLESH_TRACE, this->get_position(), 0, 0,
                       OBJ_ANIM_FLESH_TRACE, 2);
        else if (this->frame == 78)
            add_object(OBJ_FLESH_TRACE, this->get_position(), 0, 0,
                       OBJ_ANIM_FLESH_TRACE, 3);
    }
    if (ban_map.get(this->get_position()) == BAN_VOID) {
        //acceletate
        this->y_add += 3072;
        if (this->y_add > 196608L)
            this->y_add = 196608L;
    } else if (ban_map.get(this->get_position()) == BAN_WATER) {
        if (this->x_add < 0) {
            if (this->x_add < -65536L)
                this->x_add = -65536L;
            this->x_add += 1024;
            if (this->x_add > 0)
                this->x_add = 0;
        } else {
            if (this->x_add > 65536L)
                this->x_add = 65536L;
            this->x_add -= 1024;
            if (this->x_add < 0)
                this->x_add = 0;
        }
        this->y_add += 1024;
        if (this->y_add < -65536L)
            this->y_add = -65536L;
        if (this->y_add > 65536L)
            this->y_add = 65536L;
    }
    this->x += this->x_add;
    if ((this->y >> 16) > 0 && (ban_map.get(this->get_position()) == BAN_SOLID ||
                                      ban_map.get(this->get_position()) == BAN_ICE)) {
        if (this->x_add < 0) {
            this->x = (((this->x >> 16) + 16) & 0xfff0) << 16;
            this->x_add = -this->x_add >> 2;
        } else {
            this->x = ((((this->x >> 16) - 16) & 0xfff0) + 15) << 16;
            this->x_add = -this->x_add >> 2;
        }
    }
    this->y += this->y_add;
    if ((this->x >> 16) < -5 || (this->x >> 16) > 405 || (this->y >> 16) > 260)
        this->used = 0;
    if ((this->y >> 16) > 0 && (ban_map.get(this->get_position()) != 0)) {
        if (this->y_add < 0) {
            if (ban_map.get(this->get_position()) != 2) {
                this->y = (((this->y >> 16) + 16) & 0xfff0) << 16;
                this->x_add >>= 2;
                this->y_add = -this->y_add >> 2;
            }
        } else {
            if (ban_map.get(this->get_position()) == 1) {
                if (this->y_add > 131072L) {
                    this->y = ((((this->y >> 16) - 16) & 0xfff0) + 15) << 16;
                    this->x_add >>= 2;
                    this->y_add = -this->y_add >> 2;
                } else {
                    if (rnd(100) < 10) {
                        int s1 = rnd(4) - 2;
                        add_leftovers(0, screen_position_t{this->x >> 16, (this->y >> 16) + s1},
                                      this->frame, &object_gobs, leftovers);
                        add_leftovers(1, screen_position_t{this->x >> 16, (this->y >> 16) + s1},
                                      this->frame, &object_gobs, leftovers);
                    }
                    this->used = 0;
                }
            } else if (ban_map.get(this->get_position()) == 3) {
                this->y = ((((this->y >> 16) - 16) & 0xfff0) + 15) << 16;
                if (this->y_add > 131072L)
                    this->y_add = -this->y_add >> 2;
                else
                    this->y_add = 0;
            }
        }
    }
    if (this->x_add < 0 && this->x_add > -16384)
        this->x_add = -16384;
    if (this->x_add > 0 && this->x_add < 16384)
        this->x_add = 16384;
}

void object_t::update_fur() {
    if (rnd(100) < 30)
        add_object(OBJ_FLESH_TRACE, screen_position_t{this->x >> 16, this->y >> 16}, 0, 0,
                   OBJ_ANIM_FLESH_TRACE, 0);
    if (ban_map.get(this->get_position()) == 0) {
        this->y_add += 3072;
        if (this->y_add > 196608L)
            this->y_add = 196608L;
    } else if (ban_map.get(this->get_position()) == 2) {
        if (this->x_add < 0) {
            if (this->x_add < -65536L)
                this->x_add = -65536L;
            this->x_add += 1024;
            if (this->x_add > 0)
                this->x_add = 0;
        } else {
            if (this->x_add > 65536L)
                this->x_add = 65536L;
            this->x_add -= 1024;
            if (this->x_add < 0)
                this->x_add = 0;
        }
        this->y_add += 1024;
        if (this->y_add < -65536L)
            this->y_add = -65536L;
        if (this->y_add > 65536L)
            this->y_add = 65536L;
    }
    this->x += this->x_add;
    if ((this->y >> 16) > 0 && (ban_map.get(this->get_position()) == 1 ||
                                      ban_map.get(this->get_position()) == 3)) {
        if (this->x_add < 0) {
            this->x = (((this->x >> 16) + 16) & 0xfff0) << 16;
            this->x_add = -this->x_add >> 2;
        } else {
            this->x = ((((this->x >> 16) - 16) & 0xfff0) + 15) << 16;
            this->x_add = -this->x_add >> 2;
        }
    }
    this->y += this->y_add;
    if ((this->x >> 16) < -5 || (this->x >> 16) > 405 || (this->y >> 16) > 260)
        this->used = 0;
    if ((this->y >> 16) > 0 && (ban_map.get(this->get_position()) != 0)) {
        if (this->y_add < 0) {
            if (ban_map.get(this->get_position()) != 2) {
                this->y = (((this->y >> 16) + 16) & 0xfff0) << 16;
                this->x_add >>= 2;
                this->y_add = -this->y_add >> 2;
            }
        } else {
            if (ban_map.get(this->get_position()) == 1) {
                if (this->y_add > 131072L) {
                    this->y = ((((this->y >> 16) - 16) & 0xfff0) + 15) << 16;
                    this->x_add >>= 2;
                    this->y_add = -this->y_add >> 2;
                } else
                    this->used = 0;
            } else if (ban_map.get(this->get_position()) == 3) {
                this->y = ((((this->y >> 16) - 16) & 0xfff0) + 15) << 16;
                if (this->y_add > 131072L)
                    this->y_add = -this->y_add >> 2;
                else
                    this->y_add = 0;
            }
        }
    }
    if (this->x_add < 0 && this->x_add > -16384)
        this->x_add = -16384;
    if (this->x_add > 0 && this->x_add < 16384)
        this->x_add = 16384;
}

void add_smoke(const player_t& player) {
    add_object(OBJ_SMOKE,
               screen_position_t{player.get_position()} +
               screen_position_t{2 + rnd(9), 13 + rnd(5)}
               , 0,
               -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
}

void add_jetpack_smoke(const player_t& player) {
    add_object(OBJ_SMOKE, screen_position_t{player.get_position()} + screen_position_t{6 + rnd(5), 10 + rnd(5)},
               0, 16384 + rnd(8192), OBJ_ANIM_SMOKE, 0);
}

void add_object(int type, const screen_position_t& position, int x_add, int y_add, int anim, int frame) {

    for ( auto& object : objects ) {
        if (object.used == 0) {
            object = object_t{type, position, x_add, y_add, anim, frame};
            return;
        }
    }

    objects.emplace_back(type, position, x_add, y_add, anim, frame);

}