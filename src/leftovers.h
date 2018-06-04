//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_LEFTOVERS_H
#define JUMPNBUMP_LEFTOVERS_H

#include <vector>

struct gob_t;
struct pob_t;

const int NUM_LEFTOVERS = 50;

struct leftovers_t {

    struct {
        short num_pobs;
        std::vector<pob_t> pobs{NUM_LEFTOVERS};
    } page[2];
};
#endif //JUMPNBUMP_LEFTOVERS_H
