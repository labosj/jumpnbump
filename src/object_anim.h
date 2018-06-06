//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_ANIM_H
#define JUMPNBUMP_OBJECT_ANIM_H

#include "anim_frame_t.h"

struct object_anim_t {
    int num_frames;
    int restart_frame;
    anim_frame_t frame[10];
};

#endif //JUMPNBUMP_OBJECT_ANIM_H
