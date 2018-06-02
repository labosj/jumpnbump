//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_ANIM_H
#define JUMPNBUMP_OBJECT_ANIM_H

struct object_anim_t {
    int num_frames;
    int restart_frame;
    struct {
        int image;
        int ticks;
    } frame[10];
};

#endif //JUMPNBUMP_OBJECT_ANIM_H
