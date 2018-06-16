/*
 * globals.h
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

#ifndef __GLOBALS_H
#define __GLOBALS_H

#include <SFML/Window/Event.hpp>

struct main_info_t;
struct gob_t;
#include <string>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _WIN32
#include <strings.h>
#endif
#include <time.h>
#include <math.h>

#ifdef _WIN32
# define WIN32_LEAN_AND_MEAN
# include <windows.h>
# include <sys/stat.h>
# include <io.h>
# include "SDL.h"
# if USE_SDL_MIXER
#  include "SDL_mixer.h"
# endif
#else
#  include <sys/stat.h>
#include "player_t.h"
#include "anim_t.h"

#include "object_t.h"
#include "joy_t.h"


# endif

#define JNB_END_SCORE 100


void tellServerPlayerMoved(int playerid, int movement_type, int newval);
#define MOVEMENT_LEFT  1
#define MOVEMENT_RIGHT 2
#define MOVEMENT_UP    3

#define KEY_PL1_LEFT	sf::Keyboard::Key::Left
#define KEY_PL1_RIGHT	sf::Keyboard::Key::Right
#define KEY_PL1_JUMP	sf::Keyboard::Key::Up
#define KEY_PL2_LEFT	sf::Keyboard::Key::A
#define KEY_PL2_RIGHT	sf::Keyboard::Key::D
#define KEY_PL2_JUMP	sf::Keyboard::Key::W
#define KEY_PL3_LEFT	sf::Keyboard::Key::J
#define KEY_PL3_RIGHT	sf::Keyboard::Key::L
#define KEY_PL3_JUMP	sf::Keyboard::Key::I
#define KEY_PL4_LEFT	sf::Keyboard::Key::Numpad4
#define KEY_PL4_RIGHT	sf::Keyboard::Key::Numpad6
#define KEY_PL4_JUMP	sf::Keyboard::Key::Numpad8

#define OBJ_ANIM_SPRING 0
#define OBJ_ANIM_SPLASH 1
#define OBJ_ANIM_SMOKE 2
#define OBJ_ANIM_YEL_BUTFLY_RIGHT 3
#define OBJ_ANIM_YEL_BUTFLY_LEFT 4
#define OBJ_ANIM_PINK_BUTFLY_RIGHT 5
#define OBJ_ANIM_PINK_BUTFLY_LEFT 6
#define OBJ_ANIM_FLESH_TRACE 7


int init_level();
int init_program(int argc, char *argv[]);

/* input.c */

void init_inputs(main_info_t&);

int key_pressed(int key);
int addkey(unsigned int key);


#endif
