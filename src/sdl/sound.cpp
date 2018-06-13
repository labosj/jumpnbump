/*
 * sound.c
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 * 
 * Copyright (C) 2001 Chuck Mason <cemason@users.sourceforge.net>
 *
 * Copyright (C) 2002 Florian Schulze <crow@icculus.org>
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




/*

void dj_play_sfx(unsigned char sfx_num, unsigned short freq, char volume, char panning,
				 char channel)
{
	int slot;


	if (channel<0) {
		for (slot=0; slot<MAX_CHANNELS; slot++)
			if (channelinfo[slot].data==NULL)
				break;
		if (slot>=MAX_CHANNELS)
			return;
	} else
		slot = channel;

	SDL_LockAudio();
	addsfx((short *)sounds[sfx_num].buf, sounds[sfx_num].length, sounds[sfx_num].loop, freq, slot);
	updateSoundParams(slot, volume*2);
	SDL_UnlockAudio();
}

*/