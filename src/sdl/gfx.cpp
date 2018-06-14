/*
 * gfx.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 *
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
 *
 * Copyright (C) 2015 Côme Chilliet <come@chilliet.eu>
 *
 * This file is part of Jump 'n Bump.
 *
 * Jump 'n Bump is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * Jump 'n Bump is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "src/globals.h"
#include "SDL_endian.h"
#include <SDL.h>
#include <fstream>
#include "gfx.h"


#include "jumpnbump64.xpm"
#include "src/gob_t.h"
#include "src/pob_t.h"

SDL_Surface *icon;

int screen_width=400;
int screen_height=256;
int screen_pitch=400;

static int drawing_enable = 0;

static void *mask = NULL;

void put_block(int x, int y, int width, int height, unsigned char *buffer)
{
/*
	int h;
	unsigned char *vga_ptr, *buffer_ptr;

	assert(drawing_enable==1);


	if (x < 0)
		x = 0;
	if (y < 0)
		y = 0;
	if (y + height >= screen_height)
		height = screen_height - y;
	if (x + width >= screen_width)
		width = screen_width - x;
	if (width<=0)
		return;
	if(height<=0)
		return;

	vga_ptr = get_vgaptr(x, y);
	buffer_ptr = buffer;
	for (h = 0; h < height; h++) {
		memcpy(vga_ptr, buffer_ptr, width);
		vga_ptr += screen_pitch;
		buffer_ptr += width;
	}
 */
}




void put_pob(const pob_t& pob, int use_mask)
{
/*

	int c1, c2;
	int pob_x, pob_y;
	int width, height;
	int draw_width, draw_height;
	int colour;
	unsigned char *vga_ptr;
	unsigned char *pob_ptr;
	unsigned char *mask_ptr;

	int x = pob.position.x;
	int y = pob.position.y;
	assert(drawing_enable==1);
	assert(gob);
	assert(image>=0);

		width = draw_width =  pob.pob_data->images[pob.image].width;
		height = draw_height = pob.pob_data->images[pob.image].height;
		x -= pob.pob_data->images[pob.image].hs_x;
		y -= pob.pob_data->images[pob.image].hs_y;

	if ((x + width) <= 0 || x >= screen_width)
		return;
	if ((y + height) <= 0 || y >= screen_height)
		return;

	pob_x = 0;
	pob_y = 0;
	if (x < 0) {
		pob_x -= x;
		draw_width += x;
		x = 0;
	}
	if ((x + width) > screen_width)
		draw_width -= x + width - screen_width;
	if (y < 0) {
		pob_y -= y;
		draw_height += y;
		y = 0;
	}
	if ((y + height) > screen_height)
		draw_height -= y + height - screen_height;

	vga_ptr = get_vgaptr(x, y);
	pob_ptr = ((unsigned char *)pob.pob_data->images[pob.image].data) + ((pob_y * width) + pob_x);
	mask_ptr = ((unsigned char *)mask) + ((y * screen_pitch) + (x));
	for (c1 = 0; c1 < draw_height; c1++) {
		for (c2 = 0; c2 < draw_width; c2++) {
			colour = *mask_ptr;
			if (use_mask == 0 || (use_mask == 1 && colour == 0)) {
				colour = *pob_ptr;
				if (colour != 0) {
					*vga_ptr = colour;
				}
			}
			vga_ptr++;
			pob_ptr++;
			mask_ptr++;
		}
		pob_ptr += width - c2;
		vga_ptr += (screen_width - c2);
		mask_ptr += (screen_width - c2);
	}
*/
}

int register_gob(unsigned char *handle, gob_t &gob, int len)
{
	/*
	unsigned char *gob_data;
	int i;

	gob_data = reinterpret_cast<unsigned char*>(malloc(len));
	memcpy(gob_data, handle, len);

	auto num_images = (short)((gob_data[0]) + (gob_data[1] << 8));

	for (i=0; i< num_images; i++) {

		auto offset = (gob_data[i*4+2]) + (gob_data[i*4+3] << 8) + (gob_data[i*4+4] << 16) + (gob_data[i*4+5] << 24);

		auto image = image_t{};
		image.width = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
		image.height = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
		image.hs_x = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
		image.hs_y = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;


		auto image_size = image.width * image.height;
		image.orig_data =  malloc(image_size);

		memcpy(image.orig_data, &gob_data[offset], image_size);

		image.data = (unsigned short *)image.orig_data;

		gob.images.push_back(image);

	}
	free(gob_data);
	return 0;
	 */
}