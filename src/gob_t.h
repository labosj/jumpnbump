//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_GOB_H
#define JUMPNBUMP_GOB_H

#include <vector>

class image_t {
public:
	int width;
	int height;
	int hs_x;
	int hs_y;
	void* data;
	void* orig_data;
};
/**
 * Maybe this stands for graphic object
 */
struct gob_t{
	std::vector<image_t> images;
};

extern gob_t rabbit_gobs;
extern gob_t font_gobs;
extern gob_t object_gobs;
extern gob_t number_gobs;

#endif //JUMPNBUMP_GOB_H
