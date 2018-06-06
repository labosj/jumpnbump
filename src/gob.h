//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_GOB_H
#define JUMPNBUMP_GOB_H

struct gob_t{
	int num_images;
	int *width;
	int *height;
	int *hs_x;
	int *hs_y;
	void **data;
	void **orig_data;
};

extern gob_t rabbit_gobs;
extern gob_t font_gobs;
extern gob_t object_gobs;
extern gob_t number_gobs;

#endif //JUMPNBUMP_GOB_H
