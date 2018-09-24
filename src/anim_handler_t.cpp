//
// Created by edwin on 09-06-18.
//


#include "anim_t.h"
#include "anim_handler_t.h"

void anim_handler_t::animate(const std::vector<anim_t>& player_anims, int offset) {

    const auto& anim = player_anims[this->anim];

    this->frame_tick++;
    if (this->frame_tick >= anim.frame[this->frame].ticks) {
        this->frame++;
        if (this->frame >= anim.frame.size()) {
            this->frame = anim.restart_frame;
        }
        this->frame_tick = 0;
    }

    this->image = anim.frame[this->frame].image + offset;
}