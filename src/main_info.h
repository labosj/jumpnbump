//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_MAIN_INFO_H
#define JUMPNBUMP_MAIN_INFO_H

#include <string>
#include <vector>
#include "pobs_t.h"

struct gob_t;

struct main_info_t {

	bool gore = true;
	std::string error_str;

	pobs_t pobs;

	~main_info_t();
};

extern main_info_t main_info;
#endif //JUMPNBUMP_MAIN_INFO_H
