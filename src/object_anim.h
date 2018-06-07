//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_ANIM_H
#define JUMPNBUMP_OBJECT_ANIM_H

#include "anim_frame_t.h"
#include <vector>

struct object_anim_t {
    int restart_frame;
    std::vector<anim_frame_t> frame;
};

#endif //JUMPNBUMP_OBJECT_ANIM_H
