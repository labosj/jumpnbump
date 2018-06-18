//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_OBJECT_ANIM_H
#define JUMPNBUMP_OBJECT_ANIM_H

#include "anim_frame_t.h"
#include <vector>

#define OBJ_ANIM_SPRING 0
#define OBJ_ANIM_SPLASH 1
#define OBJ_ANIM_SMOKE 2
#define OBJ_ANIM_YEL_BUTFLY_RIGHT 3
#define OBJ_ANIM_YEL_BUTFLY_LEFT 4
#define OBJ_ANIM_PINK_BUTFLY_RIGHT 5
#define OBJ_ANIM_PINK_BUTFLY_LEFT 6
#define OBJ_ANIM_FLESH_TRACE 7


struct anim_t {
    int restart_frame;
    std::vector<anim_frame_t> frame;
};



#endif //JUMPNBUMP_OBJECT_ANIM_H
