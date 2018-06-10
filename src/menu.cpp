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

int menu_init(main_info_t& main_info)
{

 	players.clear();
	for (auto c1 = 0; c1 < 4; c1++) {
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

	return 0;

}

