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

#include <stdio.h>
#include <stdlib.h>

#ifdef DOS
#include <pc.h>
#include <dpmi.h>
#include <go32.h>
#include <sys/nearptr.h>
#endif

#define DJ_SD_TYPE_NOSOUND 0
#define DJ_SD_TYPE_SBLASTER 1

#define DJ_SFX_TYPE_SSS 0
#define DJ_SFX_TYPE_WAV 1
#define DJ_SFX_TYPE_SMP 2


typedef struct dj_hardware_info {
	char sd_type;
  short sd_version;
	short port;
	char irq;
	char dma;
} dj_hardware_info;

typedef struct dj_mixing_info {
  char sfx_volume, num_sfx_channels;
  char mod_volume, num_mod_channels;
	char stereo_mix, auto_mix;
  unsigned short mixing_freq;
  unsigned short dma_time, dmabuf_len;
  char cur_dmabuf;
	unsigned long dmabuf_address[2];
  char *mixed_buf;
} dj_mixing_info;

typedef struct sfx_data {
  char priority;
  unsigned short default_freq;
  char default_volume;
  unsigned long length;
  char loop;
  unsigned long loop_start, loop_length;
  unsigned char *buf;
} sfx_data;

typedef struct dj_mod_info {
  char num_channels;
	char speed;
	short bpm;
	char order_pos;
	char pat_pos;
	char name[20];
	struct {
		char name[22];
		unsigned short length;
		char finetune;
		char volume;
		unsigned short loop_start;
		unsigned short loop_length;
		char *buf;
	} samples[31];
	char song_length;
	char num_pat;
	char pat_order[128];
  char *pat[128];
} dj_mod_info;


extern char dj_init(main_info_t&);
extern void dj_deinit(main_info_t&);

extern unsigned short dj_set_mixing_freq(unsigned short freq);

extern char dj_set_num_sfx_channels(char num_channels);
extern void dj_set_sfx_volume(main_info_t&, char volume);
extern void dj_play_sfx(main_info_t&, unsigned char sfx_num, unsigned short freq, char volume, char panning, unsigned short delay, char channel);
extern void dj_stop_sfx_channel(main_info_t&, char channel_num);
extern char dj_load_sfx(main_info_t&, unsigned char *file_handle, char *filename, int file_length, char sfx_type, unsigned char sfx_num);
extern void dj_free_sfx(main_info_t&, unsigned char sfx_num);

extern char dj_ready_mod(main_info_t&, char mod_num);
extern char dj_start_mod(main_info_t&);
extern void dj_stop_mod(main_info_t&);
extern void dj_set_mod_volume(main_info_t&, char volume);


