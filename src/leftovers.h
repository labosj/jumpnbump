//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_LEFTOVERS_H
#define JUMPNBUMP_LEFTOVERS_H

#include <vector>

struct gob_t;
struct pob_t;

struct leftovers_t {

    struct {
        std::vector<pob_t> pobs;
    } page[2];
};
#endif //JUMPNBUMP_LEFTOVERS_H
