//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_LEFTOVERS_H
#define JUMPNBUMP_LEFTOVERS_H

struct gob_t;

const int NUM_LEFTOVERS = 50;

struct leftovers_t {

    struct {
        short num_pobs;
        struct {
            int x, y;
            int image;
            gob_t *pob_data;
        } pobs[NUM_LEFTOVERS];
    } page[2];
};
#endif //JUMPNBUMP_LEFTOVERS_H
