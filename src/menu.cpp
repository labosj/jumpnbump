/*
 * menu.c
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

#include "globals.h"
#include "menu.h"
#include "data.h"
#include "draw.h"
#include "util.h"
#include "sdl/gfx.h"
#include "gob_t.h"

char menu_pal[768];
char menu_cur_pal[768];

const char *message[] = {
	"Jump 'n Bump " JNB_VERSION,
	"by Brainchild Design in 1998.",
	"Code by Mattias Brynervall.",
	"Graphics by Martin Magnusson",
	"and Andreas Brynervall.",
	"Music by Anders Nilsson.",
	"Linux port by Chuck Mason.",
	"New SDL port by Florian Schulze.",
	"Port to SDL2 by Come Chilliet.",
	"http://www.icculus.org/jumpnbump/",
	"Ryan C. Gordon made networking possible again!",
	"Visit our homepage at:",
	"http://www.algonet.se/~mattiasb",
	"Jump 'n Bump is e-mailware.",
	"That means you're supposed to send us an e-mail!",
	"Write for example where you're from",
	"and what you thought about this game.",
	"If you do that, you will greatly encourage us",
	"to make more games for you!",
	"Send your e-mail to: matbr656@student.liu.se",
	"Oh, and by the way, there're a lot of secrets!",
	"If you can't find them, you'd better ask us...",
	"If you'd like to contact a specific member of BCD,",
	"these are their e-mail addresses:",
	"Andreas Brynervall: andreasb@acc.umu.se",
	"Mattias Brynervall: matbr656@student.liu.se",
	"Martin Magnusson: marma102@student.liu.se",
	"Anders Nilsson: equel@swipnet.se",
	""
};

int menu_init(main_info_t& main_info)
{
	int c1;

	fillpalette(0, 0, 0);

	if (read_pcx("/home/edwin/Projects/jumpnbump/data/menu.pcx", background_pic, JNB_WIDTH*JNB_HEIGHT, menu_pal) != 0) {
        main_info.error_str = "Error loading 'menu.pcx', aborting...\n";
		return 1;
	}

	if (read_pcx("/home/edwin/Projects/jumpnbump/data/menumask.pcx", mask_pic, JNB_WIDTH*JNB_HEIGHT, 0) != 0) {
		main_info.error_str = "Error loading 'menumask.pcx', aborting...\n";
		return 1;
	}
	memset(menu_cur_pal, 0, 768);

	/* fix dark font */
	for (c1 = 0; c1 < 16; c1++) {
		menu_pal[(240 + c1) * 3 + 0] = c1 << 2;
		menu_pal[(240 + c1) * 3 + 1] = c1 << 2;
		menu_pal[(240 + c1) * 3 + 2] = c1 << 2;
	}

	register_background(background_pic);
	register_mask(mask_pic);
 	players.clear();
	for (c1 = 0; c1 < 4; c1++) {
		//create bunnies randomly in the menu screen
		auto player = player_t{c1};
		player.position = screen_position_t{rnd(150), (160L + c1 * 2)};
		player.x_add = 0;
		player.y_add = 0;
		player.direction = rnd(2);
		player.jump_ready = 1;
		player.anim_handler.anim = 0;
		player.anim_handler.frame = 0;
		player.anim_handler.frame_tick = 0;
		player.anim_handler.image = player_anims[player.anim_handler.anim].frame[player.anim_handler.frame].image;

		players.push_back(player);
	}

	objects.clear();

	main_info.page_info[0].pobs.clear();
	main_info.page_info[1].pobs.clear();

	return 0;

}

