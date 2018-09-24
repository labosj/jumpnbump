//
// Created by edwin on 09-06-18.
//

#ifndef JUMPNBUMP_ANIM_HANDLER_T_H
#define JUMPNBUMP_ANIM_HANDLER_T_H

#include <vector>

class anim_t;
class player_t;

class anim_handler_t {
public:
    int anim;
    int frame;
    int image;
    int frame_tick;

public:
    void set_anim(int anim) {
        this->anim = anim;
        this->frame = 0;
        this->frame_tick = 0;
    }

    void animate(const std::vector<anim_t>& player_anims, int offset);
};


#endif //JUMPNBUMP_ANIM_HANDLER_T_H
