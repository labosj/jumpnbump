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

#define NUM_MESSAGES (sizeof(message)/sizeof(char *))

int menu(main_info_t& main_info, unsigned char* datafile_buffer, leftovers_t& leftovers)
{
	int c1;
	int esc_pressed;
	int end_loop_flag, new_game_flag, fade_flag;
	int mod_vol = 0, mod_fade_direction = 0;
	int cur_message;
	int fade_dir, fade_count;
	char fade_pal[48];
	int update_count;

	if (menu_init(main_info, datafile_buffer) != 0)
		return 1;

	/* After a game, we have to release the keys, cause AI players
	 * can still be using them */
	addkey((KEY_PL1_LEFT & 0x7fff) | 0x8000);
	addkey((KEY_PL2_LEFT & 0x7fff) | 0x8000);
	addkey((KEY_PL3_LEFT & 0x7fff) | 0x8000);
	addkey((KEY_PL4_LEFT & 0x7fff) | 0x8000);

	addkey((KEY_PL1_RIGHT & 0x7fff) | 0x8000);
	addkey((KEY_PL2_RIGHT & 0x7fff) | 0x8000);
	addkey((KEY_PL3_RIGHT & 0x7fff) | 0x8000);
	addkey((KEY_PL4_RIGHT & 0x7fff) | 0x8000);

	addkey((KEY_PL1_JUMP & 0x7fff) | 0x8000);
	addkey((KEY_PL2_JUMP & 0x7fff) | 0x8000);
	addkey((KEY_PL3_JUMP & 0x7fff) | 0x8000);
	addkey((KEY_PL4_JUMP & 0x7fff) | 0x8000);

	mod_vol = 0;
	mod_fade_direction = 1;
	dj_ready_mod(main_info, MOD_MENU, datafile_buffer);
	dj_set_mod_volume(main_info, (char)mod_vol);
	dj_set_sfx_volume(main_info, 64);
	dj_start_mod(main_info);

	memset(fade_pal, 0, 48);
	setpalette(240, 16, fade_pal);

	fade_dir = 0;
	fade_count = 0;
	cur_message = NUM_MESSAGES - 1;

	main_info.page_info[0].pobs.clear();
	main_info.page_info[1].pobs.clear();
	main_info.view_page = 0;
	main_info.draw_page = 1;

	esc_pressed = key_pressed(1);
	end_loop_flag = new_game_flag = 0;

	update_count = 1;
	while (true) {

		while (update_count) {

			if (key_pressed(1) == 1 && esc_pressed == 0) {
				end_loop_flag = 1;
				new_game_flag = 0;
				memset(menu_pal, 0, 768);
				mod_fade_direction = 0;
			} else if (key_pressed(1) == 0)
				esc_pressed = 0;

			update_player_actions();
			for (c1 = 0; c1 < players.size(); c1++) {
				auto screen_position = screen_position_t{players[c1].get_position()};
				if (end_loop_flag == 1 && new_game_flag == 1) {
					if ((screen_position.x) > (165 + c1 * 2)) {
						if (players[c1].x_add < 0)
							players[c1].x_add += 16384;
						else
							players[c1].x_add += 12288;
						if (players[c1].x_add > 98304L)
							players[c1].x_add = 98304L;
						players[c1].direction = 0;
						if (players[c1].anim == 0) {
							players[c1].anim = 1;
							players[c1].frame = 0;
							players[c1].frame_tick = 0;
							players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
						}
						players[c1].enabled = 1;
					}
					if (!players[c1].action_up) {
						if (players[c1].y_add < 0) {
							players[c1].y_add += 32768;
							if (players[c1].y_add > 0)
								players[c1].y_add = 0;
						}
					}
					players[c1].y_add += 12288;
					if (players[c1].y_add > 36864 && players[c1].anim != 3) {
						players[c1].anim = 3;
						players[c1].frame = 0;
						players[c1].frame_tick = 0;
						players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
					}
					players[c1].position.y += players[c1].y_add;
					if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
						if ((players[c1].position.y >> 16) > (160 + c1 * 2)) {
							players[c1].position.y = (160L + c1 * 2) << 16;
							players[c1].y_add = 0;
							if (players[c1].anim != 0 && players[c1].anim != 1) {
								players[c1].anim = 0;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
							}
						}
					} else {
						if ((players[c1].position.y >> 16) > (138 + c1 * 2)) {
							players[c1].position.y = (138L + c1 * 2) << 16;
							players[c1].y_add = 0;
							if (players[c1].anim != 0 && players[c1].anim != 1) {
								players[c1].anim = 0;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
							}
							if (!players[c1].action_up)
								players[c1].jump_ready = 1;
						}
					}
					players[c1].position.x += players[c1].x_add;
					if ((players[c1].position.y >> 16) > (138 + c1 * 2)) {
						if ((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (190 + c1 * 2)) {
							players[c1].position.x = (165L + c1 * 2) << 16;
							players[c1].x_add = 0;
						}
						if ((players[c1].position.x >> 16) > (190 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) {
							players[c1].position.x = (208L + c1 * 2) << 16;
							players[c1].x_add = 0;
						}
					}
				} else {
					if (players[c1].action_left && players[c1].action_right) {
						if (players[c1].direction == 1) {
							if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
								if (players[c1].x_add > 0) {
									players[c1].x_add -= 16384;
									if ((players[c1].position.y >> 16) >= (160 + c1 * 2))
										add_smoke(players[c1]);;
								} else
									players[c1].x_add -= 12288;
							}
							if ((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) {
								if (players[c1].x_add > 0) {
									players[c1].x_add -= 16384;
									if ((players[c1].position.y >> 16) >= (138 + c1 * 2))
										add_smoke(players[c1]);;
								} else
									players[c1].x_add -= 12288;
							}
							if (players[c1].x_add < -98304L)
								players[c1].x_add = -98304L;
							players[c1].direction = 1;
							if (players[c1].anim == 0) {
								players[c1].anim = 1;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
							}
						} else {
							if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
								if (players[c1].x_add < 0) {
									players[c1].x_add += 16384;
									if ((players[c1].position.y >> 16) >= (160 + c1 * 2))
										add_object(OBJ_SMOKE, players[c1].get_position() + screen_position_t{2 + rnd(9), 13 + rnd(5)}, 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
								} else
									players[c1].x_add += 12288;
							}
							if ((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) {
								if (players[c1].x_add < 0) {
									players[c1].x_add += 16384;
									if ((players[c1].position.y >> 16) >= (138 + c1 * 2))
										add_object(OBJ_SMOKE, players[c1].get_position() + screen_position_t{2 + rnd(9), 13 + rnd(5)}, 0, -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
								} else
									players[c1].x_add += 12288;
							}
							if (players[c1].x_add > 98304L)
								players[c1].x_add = 98304L;
							players[c1].direction = 0;
							if (players[c1].anim == 0) {
								players[c1].anim = 1;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
							}
						}
					} else if (players[c1].action_left) {
						if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
							if (players[c1].x_add > 0) {
								players[c1].x_add -= 16384;
								if ((players[c1].position.y >> 16) >= (160 + c1 * 2))
									add_smoke(players[c1]);;
							} else
								players[c1].x_add -= 12288;
						}
						if ((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) {
							if (players[c1].x_add > 0) {
								players[c1].x_add -= 16384;
								if ((players[c1].position.y >> 16) >= (138 + c1 * 2))
									add_smoke(players[c1]);;
							} else
								players[c1].x_add -= 12288;
						}
						if (players[c1].x_add < -98304L)
							players[c1].x_add = -98304L;
						players[c1].direction = 1;
						if (players[c1].anim == 0) {
							players[c1].anim = 1;
							players[c1].frame = 0;
							players[c1].frame_tick = 0;
							players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
						}
					} else if (players[c1].action_right) {
						if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
							if (players[c1].x_add < 0) {
								players[c1].x_add += 16384;
								if ((players[c1].position.y >> 16) >= (160 + c1 * 2))
									add_smoke(players[c1]);;
							} else
								players[c1].x_add += 12288;
						}
						if ((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) {
							if (players[c1].x_add < 0) {
								players[c1].x_add += 16384;
								if ((players[c1].position.y >> 16) >= (138 + c1 * 2))
									add_smoke(players[c1]);;
							} else
								players[c1].x_add += 12288;
						}
						if (players[c1].x_add > 98304L)
							players[c1].x_add = 98304L;
						players[c1].direction = 0;
						if (players[c1].anim == 0) {
							players[c1].anim = 1;
							players[c1].frame = 0;
							players[c1].frame_tick = 0;
							players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
						}
					} else {
						if (((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) && (players[c1].position.y >> 16) >= (160 + c1 * 2)) {
							if (players[c1].x_add < 0) {
								players[c1].x_add += 16384;
								if (players[c1].x_add > 0)
									players[c1].x_add = 0;
								add_smoke(players[c1]);
							} else if (players[c1].x_add > 0) {
								players[c1].x_add -= 16384;
								if (players[c1].x_add < 0)
									players[c1].x_add = 0;
								add_smoke(players[c1]);
							}
						}
						if ((((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) && (players[c1].position.y >> 16) >= (138 + c1 * 2))) {
							if (players[c1].x_add < 0) {
								players[c1].x_add += 16384;
								if (players[c1].x_add > 0)
									players[c1].x_add = 0;
								add_smoke(players[c1]);
							} else if (players[c1].x_add > 0) {
								players[c1].x_add -= 16384;
								if (players[c1].x_add < 0)
									players[c1].x_add = 0;
								add_smoke(players[c1]);
							}
						}
						if (players[c1].anim == 1) {
							players[c1].anim = 0;
							players[c1].frame = 0;
							players[c1].frame_tick = 0;
							players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
						}
					}
					if ((players[c1].jump_ready == 1) && players[c1].action_up) {
						if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
							if ((players[c1].position.y >> 16) >= (160 + c1 * 2)) {
								players[c1].y_add = -280000L;
								players[c1].anim = 2;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
								players[c1].jump_ready = 0;
								dj_play_sfx(main_info, SFX_JUMP, (unsigned short) (SFX_JUMP_FREQ + rnd(2000) - 1000),
											64, 0, -1);
							}
						} else {
							if ((players[c1].position.y >> 16) >= (138 + c1 * 2)) {
								players[c1].y_add = -280000L;
								players[c1].anim = 2;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
								players[c1].jump_ready = 0;
								dj_play_sfx(main_info, SFX_JUMP, (unsigned short) (SFX_JUMP_FREQ + rnd(2000) - 1000),
											64, 0, -1);
							}
						}
					}
					if (!players[c1].action_up) {
						if (players[c1].y_add < 0) {
							players[c1].y_add += 32768;
							if (players[c1].y_add > 0)
								players[c1].y_add = 0;
						}
					}
					if (!players[c1].action_up)
						players[c1].jump_ready = 1;
					players[c1].y_add += 12288;
					if (players[c1].y_add > 36864 && players[c1].anim != 3) {
						players[c1].anim = 3;
						players[c1].frame = 0;
						players[c1].frame_tick = 0;
						players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
					}
					players[c1].position.y += players[c1].y_add;
					if ((players[c1].position.x >> 16) <= (165 + c1 * 2) || (players[c1].position.x >> 16) >= (208 + c1 * 2)) {
						if ((players[c1].position.y >> 16) > (160 + c1 * 2)) {
							players[c1].position.y = (160L + c1 * 2) << 16;
							players[c1].y_add = 0;
							if (players[c1].anim != 0 && players[c1].anim != 1) {
								players[c1].anim = 0;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
							}
						}
					} else {
						if ((players[c1].position.y >> 16) > (138 + c1 * 2)) {
							players[c1].position.y = (138L + c1 * 2) << 16;
							players[c1].y_add = 0;
							if (players[c1].anim != 0 && players[c1].anim != 1) {
								players[c1].anim = 0;
								players[c1].frame = 0;
								players[c1].frame_tick = 0;
								players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
							}
						}
					}
					players[c1].position.x += players[c1].x_add;
					if ((players[c1].position.x >> 16) < 0) {
						players[c1].position.x = 0;
						players[c1].x_add = 0;
					}
					if ((players[c1].position.x >> 16) > JNB_WIDTH) {
						end_loop_flag = 1;
						new_game_flag = 1;
						memset(menu_pal, 0, 768);
						mod_fade_direction = 0;
					}
					if ((players[c1].position.y >> 16) > (138 + c1 * 2)) {
						if ((players[c1].position.x >> 16) > (165 + c1 * 2) && (players[c1].position.x >> 16) < (190 + c1 * 2)) {
							players[c1].position.x = (165L + c1 * 2) << 16;
							players[c1].x_add = 0;
						}
						if ((players[c1].position.x >> 16) > (190 + c1 * 2) && (players[c1].position.x >> 16) < (208 + c1 * 2)) {
							players[c1].position.x = (208L + c1 * 2) << 16;
							players[c1].x_add = 0;
						}
					}
				}
				players[c1].frame_tick++;
				if (players[c1].frame_tick >= player_anims[players[c1].anim].frame[players[c1].frame].ticks) {
					players[c1].frame++;
					if (players[c1].frame >= player_anims[players[c1].anim].frame.size())
						players[c1].frame = player_anims[players[c1].anim].restart_frame;
					players[c1].frame_tick = 0;
				}
				players[c1].image = player_anims[players[c1].anim].frame[players[c1].frame].image + players[c1].direction * 9;
			}


			main_info.page_info[main_info.draw_page].pobs.clear();

			for (c1 = 3; c1 >= 0; c1--)
				add_pob(main_info.draw_page, players[c1].get_position(), players[c1].image + c1 * 18, &rabbit_gobs);

			update_objects();

			if (update_count == 1) {
				draw_begin();
				draw_pobs(main_info.draw_page, main_info);
				draw_end();

				if (mod_fade_direction == 1) {
					if (mod_vol < 35) {
						mod_vol++;
						dj_set_mod_volume(main_info, (char)mod_vol);
					}
				} else {
					if (mod_vol > 0) {
						mod_vol--;
						dj_set_mod_volume(main_info, (char)mod_vol);
					}
				}
			}

			fade_flag = 0;
			for (c1 = 0; c1 < 720; c1++) {
				if (menu_cur_pal[c1] < menu_pal[c1]) {
					menu_cur_pal[c1]++;
					fade_flag = 1;
				} else if (menu_cur_pal[c1] > menu_pal[c1]) {
					menu_cur_pal[c1]--;
					fade_flag = 2;
				}
			}
			if (fade_flag == 0 && end_loop_flag == 1) {
				menu_deinit(main_info);
				if (new_game_flag == 1)
					return 0;
				else
					return 1;
			}
			switch (fade_dir) {
			case 0:
				if (fade_count < 30) {
					for (c1 = 0; c1 < 48; c1++) {
						if (fade_pal[c1] > 0)
							fade_pal[c1]--;
					}
					fade_count++;
				} else {
					draw_begin();
					clear_lines(0, 220, 20, 0);
					clear_lines(1, 220, 20, 0);

					cur_message++;
					if (cur_message >= NUM_MESSAGES)
						cur_message -= NUM_MESSAGES;
					put_text(0, 200, 220, message[cur_message], 2);
					put_text(1, 200, 220, message[cur_message], 2);
					fade_dir = 1;
					fade_count = 0;
					draw_end();
				}
				break;
			case 1:
				if (fade_count < 100) {
					for (c1 = 0; c1 < 48; c1++) {
						if (fade_pal[c1] < menu_pal[c1 + 720])
							fade_pal[c1]++;
					}
					fade_count++;
				} else {
					fade_dir = 0;
					fade_count = 0;
				}
				break;
			}

			for (c1 = 0; c1 < 48; c1++) {
				if (fade_pal[c1] > menu_pal[c1 + 720])
					fade_pal[c1]--;
			}

			if (update_count == 1) {
				main_info.draw_page ^= 1;
				main_info.view_page ^= 1;

				flippage(main_info.view_page);

				wait_vrt();
			}

			if (fade_flag != 0) {
				setpalette(0, 240, menu_cur_pal);
			}

			if (update_count == 1) {
				setpalette(240, 16, fade_pal);

				draw_begin();
				redraw_pob_backgrounds(main_info.draw_page, main_info);
				draw_end();
			}

			update_count--;
		}

		update_count = intr_sysupdate();

	}

	menu_deinit(main_info);
	return 0;

}


int menu_init(main_info_t& main_info, unsigned char *datafile_buffer)
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
		auto player = player_t{};

		player.enabled = 0;
		player.position = screen_position_t{rnd(150), (160L + c1 * 2)};
		player.x_add = 0;
		player.y_add = 0;
		player.direction = rnd(2);
		player.jump_ready = 1;
		player.anim = 0;
		player.frame = 0;
		player.frame_tick = 0;
		player.image = player_anims[player.anim].frame[player.frame].image;

		players.push_back(player);
	}

	for (c1 = 0; c1 < objects.size(); c1++)
		objects[c1].used = 0;

	main_info.page_info[0].pobs.clear();
	main_info.page_info[1].pobs.clear();

	return 0;

}


void menu_deinit(main_info_t&)
{
}
