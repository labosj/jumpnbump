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

void put_pob(int page, int x, int y, int image, gob_t& gob, int mask);
int pob_width(int image, gob_t& gob);
int pob_height(int image, gob_t& gob);
int pob_hs_x(int image, gob_t& gob);
int pob_hs_y(int image, gob_t& gob);
int register_gob(unsigned char *handle, gob_t &gob, int len);


extern gob_t rabbit_gobs;
extern gob_t font_gobs;
extern gob_t object_gobs;
extern gob_t number_gobs;

#endif //JUMPNBUMP_GOB_H
