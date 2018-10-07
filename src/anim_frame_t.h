//
// Created by edwin on 06-06-18.
//

#ifndef JUMPNBUMP_ANIM_FRAME_H
#define JUMPNBUMP_ANIM_FRAME_H


class anim_frame_t {
public:
    int image;
    int ticks;

    anim_frame_t(int image, int ticks) :image(image), ticks(ticks) {}
};


#endif //JUMPNBUMP_ANIM_FRAME_H
