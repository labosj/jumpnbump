//
// Created by edwin on 02-06-18.
//

#include "object_t.h"

#include "ban_map_t.h"
#include "util.h"
#include "globals.h"
#include "gob_t.h"
#include "leftovers_t.h"
#include "objects_t.h"
#include "game_manager_t.h"


object_t::object_t(game_manager_t& game_manager, object_t::Type type, const position_t &position, int x_add, int y_add, int anim, int frame) : game_manager(game_manager) {
    this->used = 1;
    this->type = type;

    this->position = position;
    this->x_add = x_add;
    this->y_add = y_add;
    this->x_acc = 0;
    this->y_acc = 0;
    this->set_anim(anim, frame);
}

void object_t::operator=(const object_t& other) {
    this->used = other.used;
    this->type = other.type;

    this->position = other.position;
    this->x_add = other.x_add;
    this->y_add = other.y_add;
    this->x_acc = other.x_acc;
    this->y_acc = other.y_acc;
    this->anim_handler = other.anim_handler;
}

void object_t::set_anim(int anim, int frame) {


    auto& object_anims = this->game_manager.object_anims;
    this->anim_handler.anim = anim;
    this->anim_handler.frame = frame;
    this->anim_handler.frame_tick = object_anims[anim].frame[frame].ticks;
    this->anim_handler.image = object_anims[anim].frame[frame].image;
}

void object_t::advance_anim() {
    auto& object_anims = this->game_manager.object_anims;
    this->anim_handler.frame_tick--;
    if (this->anim_handler.frame_tick <= 0) {
        this->anim_handler.frame++;
        if (this->anim_handler.frame >= object_anims[this->anim_handler.anim].frame.size())
            this->used = 0;
        else {
            this->anim_handler.frame_tick = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].ticks;
            this->anim_handler.image = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].image;
        }
    }
}

void object_t::update_flesh_trace() {
    this->advance_anim();
}

void object_t::update_smoke() {
    this->position.x += this->x_add;
    this->position.y += this->y_add;
    this->advance_anim();
}

void object_t::update_spring() {
    auto& object_anims = this->game_manager.object_anims;
    this->anim_handler.frame_tick--;
    if (this->anim_handler.frame_tick <= 0) {
        this->anim_handler.frame++;
        if (this->anim_handler.frame >= object_anims[this->anim_handler.anim].frame.size()) {
            this->anim_handler.frame--;
            this->anim_handler.frame_tick = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].ticks;
        } else {
            this->anim_handler.frame_tick = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].ticks;
            this->anim_handler.image = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].image;
        }
    }
}


void object_t::update_splash() {
    this->advance_anim();
}

void object_t::update_butterfly() {
    auto& ban_map = this->game_manager.get_stage().get_map();

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
    this->position.x += this->x_add;
    if ((this->position.x >> 16) < 16) {
        this->position.x = 16 << 16;
        this->x_add = -this->x_add >> 2;
        this->x_acc = 0;
    } else if ((this->position.x >> 16) > 350) {
        this->position.x = 350 << 16;
        this->x_add = -this->x_add >> 2;
        this->x_acc = 0;
    }
    if (ban_map.get(this->get_position()) != ban_map_t::Type::VOID) {
        if (this->x_add < 0) {
            this->position.x = (((this->position.x >> 16) + 16) & 0xfff0) << 16;
        } else {
            this->position.x = ((((this->position.x >> 16) - 16) & 0xfff0) + 15) << 16;
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
    this->position.y += this->y_add;
    if ((this->position.y >> 16) < 0) {
        this->position.y = 0;
        this->y_add = -this->y_add >> 2;
        this->y_acc = 0;
    } else if ((this->position.y >> 16) > 255) {
        this->position.y = 255 << 16;
        this->y_add = -this->y_add >> 2;
        this->y_acc = 0;
    }
    if (ban_map.get(this->get_position()) != ban_map_t::Type::VOID) {
        if (this->y_add < 0) {
            this->position.y = (((this->position.y >> 16) + 16) & 0xfff0) << 16;
        } else {
            this->position.y = ((((this->position.y >> 16) - 16) & 0xfff0) + 15) << 16;
        }
        this->y_add = -this->y_add >> 2;
        this->y_acc = 0;
    }
    if (this->type == object_t::Type::YEL_BUTFLY) {
        if (this->x_add < 0 && this->anim_handler.anim != OBJ_ANIM_YEL_BUTFLY_LEFT) {
            this->set_anim(OBJ_ANIM_YEL_BUTFLY_LEFT, 0);
        } else if (this->x_add > 0 && this->anim_handler.anim != OBJ_ANIM_YEL_BUTFLY_RIGHT) {
            this->set_anim(OBJ_ANIM_YEL_BUTFLY_RIGHT, 0);
        }
    } else {
        if (this->x_add < 0 && this->anim_handler.anim != OBJ_ANIM_PINK_BUTFLY_LEFT) {
            this->set_anim(OBJ_ANIM_PINK_BUTFLY_LEFT, 0);
        } else if (this->x_add > 0 && this->anim_handler.anim != OBJ_ANIM_PINK_BUTFLY_RIGHT) {
            this->set_anim(OBJ_ANIM_PINK_BUTFLY_RIGHT, 0);
        }
    }

    auto& object_anims = this->game_manager.object_anims;

    this->anim_handler.frame_tick--;
    if (this->anim_handler.frame_tick <= 0) {
        this->anim_handler.frame++;
        if (this->anim_handler.frame >= object_anims[this->anim_handler.anim].frame.size())
            this->anim_handler.frame = object_anims[this->anim_handler.anim].restart_frame;
        else {
            this->anim_handler.frame_tick = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].ticks;
            this->anim_handler.image = object_anims[this->anim_handler.anim].frame[this->anim_handler.frame].image;
        }
    }
}


void object_t::update_flesh() {
    auto& game_manager = this->game_manager;
    auto& ban_map = game_manager.get_stage().get_map();

    if (rnd(100) < 30) {
        if (this->anim_handler.frame == 76)
            objects.add(game_manager, object_t::Type::FLESH_TRACE, this->get_position(), 0, 0,
                       OBJ_ANIM_FLESH_TRACE, 1);
        else if (this->anim_handler.frame == 77)
            objects.add(game_manager, object_t::Type::FLESH_TRACE, this->get_position(), 0, 0,
                       OBJ_ANIM_FLESH_TRACE, 2);
        else if (this->anim_handler.frame == 78)
            objects.add(game_manager, object_t::Type::FLESH_TRACE, this->get_position(), 0, 0,
                       OBJ_ANIM_FLESH_TRACE, 3);
    }
    if (ban_map.get(this->get_position()) == ban_map_t::Type::VOID) {
        //acceletate
        this->y_add += 3072;
        if (this->y_add > 196608L)
            this->y_add = 196608L;
    } else if (ban_map.get(this->get_position()) == ban_map_t::Type::WATER) {
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
    this->position.x += this->x_add;
    if ((this->position.y >> 16) > 0 && (ban_map.get(this->get_position()) == ban_map_t::Type::SOLID ||
                                         ban_map.get(this->get_position()) == ban_map_t::Type::ICE)) {
        if (this->x_add < 0) {
            this->position.x = (((this->position.x >> 16) + 16) & 0xfff0) << 16;
            this->x_add = -this->x_add >> 2;
        } else {
            this->position.x = ((((this->position.x >> 16) - 16) & 0xfff0) + 15) << 16;
            this->x_add = -this->x_add >> 2;
        }
    }
    this->position.y += this->y_add;
    if ((this->position.x >> 16) < -5 || (this->position.x >> 16) > 405 || (this->position.y >> 16) > 260)
        this->used = 0;
    if ((this->position.y >> 16) > 0 && (ban_map.get(this->get_position()) != ban_map_t::Type::VOID)) {
        if (this->y_add < 0) {
            if (ban_map.get(this->get_position()) != ban_map_t::Type::WATER) {
                this->position.y = (((this->position.y >> 16) + 16) & 0xfff0) << 16;
                this->x_add >>= 2;
                this->y_add = -this->y_add >> 2;
            }
        } else {
            if (ban_map.get(this->get_position()) == ban_map_t::Type::SOLID) {
                if (this->y_add > 131072L) {
                    this->position.y = ((((this->position.y >> 16) - 16) & 0xfff0) + 15) << 16;
                    this->x_add >>= 2;
                    this->y_add = -this->y_add >> 2;
                } else {
                    if (rnd(100) < 10) {
                        int s1 = rnd(4) - 2;
                        game_manager.leftovers.add(screen_position_t{this->position.x >> 16, (this->position.y >> 16) + s1},
                                    this->anim_handler.frame, &(game_manager.object_gobs));
                        game_manager.leftovers.add(screen_position_t{this->position.x >> 16, (this->position.y >> 16) + s1},
                                      this->anim_handler.frame, &(game_manager.object_gobs));
                    }
                    this->used = 0;
                }
            } else if (ban_map.get(this->get_position()) == ban_map_t::Type::ICE) {
                this->position.y = ((((this->position.y >> 16) - 16) & 0xfff0) + 15) << 16;
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
    auto& ban_map = this->game_manager.get_stage().get_map();

    if (rnd(100) < 30)
        objects.add(game_manager, object_t::Type::FLESH_TRACE, this->get_position(), 0, 0,
                   OBJ_ANIM_FLESH_TRACE, 0);
    if (ban_map.get(this->get_position()) == ban_map_t::Type::VOID) {
        this->y_add += 3072;
        if (this->y_add > 196608L)
            this->y_add = 196608L;
    } else if (ban_map.get(this->get_position()) == ban_map_t::Type::WATER) {
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
    this->position.x += this->x_add;
    if ((this->position.y >> 16) > 0 && (ban_map.get(this->get_position()) == ban_map_t::Type::SOLID ||
                                         ban_map.get(this->get_position()) == ban_map_t::Type::ICE)) {
        if (this->x_add < 0) {
            this->position.x = (((this->position.x >> 16) + 16) & 0xfff0) << 16;
            this->x_add = -this->x_add >> 2;
        } else {
            this->position.x = ((((this->position.x >> 16) - 16) & 0xfff0) + 15) << 16;
            this->x_add = -this->x_add >> 2;
        }
    }
    this->position.y += this->y_add;
    if ((this->position.x >> 16) < -5 || (this->position.x >> 16) > 405 || (this->position.y >> 16) > 260)
        this->used = 0;
    if ((this->position.y >> 16) > 0 && (ban_map.get(this->get_position()) != ban_map_t::Type::VOID)) {
        if (this->y_add < 0) {
            if (ban_map.get(this->get_position()) != ban_map_t::Type::WATER) {
                this->position.y = (((this->position.y >> 16) + 16) & 0xfff0) << 16;
                this->x_add >>= 2;
                this->y_add = -this->y_add >> 2;
            }
        } else {
            if (ban_map.get(this->get_position()) == ban_map_t::Type::SOLID) {
                if (this->y_add > 131072L) {
                    this->position.y = ((((this->position.y >> 16) - 16) & 0xfff0) + 15) << 16;
                    this->x_add >>= 2;
                    this->y_add = -this->y_add >> 2;
                } else
                    this->used = 0;
            } else if (ban_map.get(this->get_position()) == ban_map_t::Type::ICE) {
                this->position.y = ((((this->position.y >> 16) - 16) & 0xfff0) + 15) << 16;
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