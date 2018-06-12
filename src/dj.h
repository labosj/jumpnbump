/*
 * dj.h
 * Copyright (C) 1998 Brainchild Design - http://brainchilddesign.com/
 * 
 * Copyright (C) 2002 Florian Schulze - crow@icculus.org
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

#ifndef JNB_DJ
#define JNB_DJ


struct sfx_data {
    unsigned long length;
  char loop;
    unsigned char *buf;
};


extern char dj_init();

extern unsigned short dj_set_mixing_freq(unsigned short freq);

extern char dj_set_num_sfx_channels(char num_channels);
extern void dj_set_sfx_volume(char volume);
extern void
dj_play_sfx(unsigned char sfx_num, unsigned short freq, char volume, char panning, char channel);
extern void dj_stop_sfx_channel(char channel_num);
extern char
dj_load_sfx(unsigned char *file_handle, int file_length, unsigned char sfx_num);

extern char dj_ready_mod(unsigned char* datafile_buffer);
extern char dj_start_mod();
extern void dj_stop_mod();
extern void dj_set_mod_volume(char volume);


#endif