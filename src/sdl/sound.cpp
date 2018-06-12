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

#include "src/globals.h"
#include <limits.h>
#include "src/main_info.h"
#ifndef _WIN32
#include <unistd.h>
#endif
#include "SDL.h"

#ifndef NO_SDL_MIXER
#include "SDL_mixer.h"
#include "../data.h"

static Mix_Music *current_music = (Mix_Music *) NULL;
#endif

sfx_data sounds[NUM_SFX];

static int SAMPLECOUNT = 512;

#define MAX_CHANNELS	32

typedef struct {
	/* loop flag */
	int loop;
	/* The channel step amount... */
	unsigned int step;
	/* ... and a 0.16 bit remainder of last step. */
	unsigned int stepremainder;
	unsigned int samplerate;
	/* The channel data pointers, start and end. */
	signed short* data;
	signed short* startdata;
	signed short* enddata;
	/* Hardware left and right channel volume lookup. */
	int leftvol;
	int rightvol;
} channel_info_t;

channel_info_t channelinfo[MAX_CHANNELS];

/* Sample rate in samples/second */
int audio_rate = 44100;
int global_sfx_volume = 0;
/*
// This function loops all active (internal) sound
//  channels, retrieves a given number of samples
//  from the raw sound data, modifies it according
//  to the current (internal) channel parameters,
//  mixes the per channel samples into the given
//  mixing buffer, and clamping it to the allowed
//  range.
//
// This function currently supports only 16bit.
*/

static void stopchan(int i)
{
	if (channelinfo[i].data) {
		memset(&channelinfo[i], 0, sizeof(channel_info_t));
	}
}


/*
// This function adds a sound to the
//  list of currently active sounds,
//  which is maintained as a given number
//  (eight, usually) of internal channels.
// Returns a handle.
*/
int addsfx(signed short *data, int len, int loop, int samplerate, int channel)
{
	stopchan(channel);

	/* We will handle the new SFX. */
	/* Set pointer to raw data. */
	channelinfo[channel].data = data;
	channelinfo[channel].startdata = data;
      
	/* Set pointer to end of raw data. */
	channelinfo[channel].enddata = channelinfo[channel].data + len - 1;
	channelinfo[channel].samplerate = samplerate;

	channelinfo[channel].loop = loop;
	channelinfo[channel].stepremainder = 0;

	return channel;
}


static void updateSoundParams(int slot, int volume)
{
	int rightvol;
	int leftvol;

	/*
	// Set stepping
	// MWM 2000-12-24: Calculates proportion of channel samplerate
	// to global samplerate for mixing purposes.
	// Patched to shift left *then* divide, to minimize roundoff errors
	// as well as to use SAMPLERATE as defined above, not to assume 11025 Hz
	*/
	channelinfo[slot].step = ((channelinfo[slot].samplerate<<16)/audio_rate);

	leftvol = volume;
	rightvol= volume;  

	/* Sanity check, clamp volume. */
	if (rightvol < 0)
		rightvol = 0;
	if (rightvol > 127)
		rightvol = 127;
    
	if (leftvol < 0)
		leftvol = 0;
	if (leftvol > 127)
		leftvol = 127;
    
	channelinfo[slot].leftvol = leftvol;
	channelinfo[slot].rightvol = rightvol;
}


void mix_sound(Uint8 *stream, int len)
{
	/* Mix current sound data. */
	/* Data, from raw sound, for right and left. */
	register int sample;
	register int    dl;
	register int    dr;

	/* Pointers in audio stream, left, right, end. */
	signed short*   leftout;
	signed short*   rightout;
	signed short*   leftend;
	/* Step in stream, left and right, thus two. */
	int       step;

	/* Mixing channel index. */
	int       chan;

	/* Left and right channel */
	/*  are in audio stream, alternating. */
	leftout = (signed short *)stream;
	rightout = ((signed short *)stream)+1;
	step = 2;

	/* Determine end, for left channel only */
	/*  (right channel is implicit). */
	leftend = leftout + (len/4)*step;

	/* Mix sounds into the mixing buffer. */
	/* Loop over step*SAMPLECOUNT, */
	/*  that is 512 values for two channels. */
	while (leftout != leftend) {
		/* Reset left/right value. */
		dl = *leftout * 256;
		dr = *rightout * 256;

		/* Love thy L2 chache - made this a loop. */
		/* Now more channels could be set at compile time */
		/*  as well. Thus loop those  channels. */
		for ( chan = 0; chan < MAX_CHANNELS; chan++ ) {
			/* Check channel, if active. */
			if (channelinfo[chan].data) {
				/* Get the raw data from the channel. */
				/* no filtering */
				/* sample = *channelinfo[chan].data; */
				/* linear filtering */
				sample = (int)(((int)channelinfo[chan].data[0] * (int)(0x10000 - channelinfo[chan].stepremainder))
					+ ((int)channelinfo[chan].data[1] * (int)(channelinfo[chan].stepremainder))) >> 16;

				/* Add left and right part */
				/*  for this channel (sound) */
				/*  to the current data. */
				/* Adjust volume accordingly. */
				dl += sample * (channelinfo[chan].leftvol * global_sfx_volume) / 128;
				dr += sample * (channelinfo[chan].rightvol * global_sfx_volume) / 128;
				/* Increment index ??? */
				channelinfo[chan].stepremainder += channelinfo[chan].step;
				/* MSB is next sample??? */
				channelinfo[chan].data += channelinfo[chan].stepremainder >> 16;
				/* Limit to LSB??? */
				channelinfo[chan].stepremainder &= 0xffff;

				/* Check whether we are done. */
				if (channelinfo[chan].data >= channelinfo[chan].enddata) {
					if (channelinfo[chan].loop) {
						channelinfo[chan].data = channelinfo[chan].startdata;
					} else {
						stopchan(chan);
					}
				}
			}
		}
  
		/* Clamp to range. Left hardware channel. */
		/* Has been char instead of short. */
		/* if (dl > 127) *leftout = 127; */
		/* else if (dl < -128) *leftout = -128; */
		/* else *leftout = dl; */

		dl = dl / 256;
		dr = dr / 256;

		if (dl > SHRT_MAX)
			*leftout = SHRT_MAX;
		else if (dl < SHRT_MIN)
			*leftout = SHRT_MIN;
		else
			*leftout = (signed short)dl;

		/* Same for right hardware channel. */
		if (dr > SHRT_MAX)
			*rightout = SHRT_MAX;
		else if (dr < SHRT_MIN)
			*rightout = SHRT_MIN;
		else
			*rightout = (signed short)dr;

		/* Increment current pointers in stream */
		leftout += step;
		rightout += step;
	}
}

/* misc handling */

char dj_init()
{
	Uint16 audio_format = MIX_DEFAULT_FORMAT;
	int audio_channels = 2;
	int audio_buffers = 4096;

	audio_buffers = SAMPLECOUNT*audio_rate/11025;

	memset(channelinfo, 0, sizeof(channelinfo));
	memset(sounds, 0, sizeof(sounds));

#ifndef NO_SDL_MIXER
	if (Mix_OpenAudio(audio_rate, audio_format, audio_channels, audio_buffers) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		main_info.no_sound = 1;
		return 1;
	}

	Mix_QuerySpec(&audio_rate, &audio_format, &audio_channels);
	printf("Opened audio at %dHz %dbit %s, %d bytes audio buffer\n", audio_rate, (audio_format & 0xFF), (audio_channels > 1) ? "stereo" : "mono", audio_buffers);

	Mix_SetMusicCMD(getenv("MUSIC_CMD"));

	Mix_SetPostMix(NULL, 0);
#else
	main_info.no_sound = 1;
	return 1;
#endif

	return 0;
}

unsigned short dj_set_mixing_freq(unsigned short freq)
{
	return freq;
}

/* sfx handling */

char dj_set_num_sfx_channels(char num_channels)
{
	return num_channels;
}

void dj_set_sfx_volume(char volume)
{
	SDL_LockAudio();
	global_sfx_volume = volume*2;
	SDL_UnlockAudio();
}

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

void dj_stop_sfx_channel(char channel_num)
{

	SDL_LockAudio();
	stopchan(channel_num);
	SDL_UnlockAudio();
}

char dj_load_sfx(unsigned char *file_handle, int file_length, unsigned char sfx_num)
{
	unsigned int i;
	unsigned char *src;
	unsigned short *dest;

	sounds[sfx_num].buf = reinterpret_cast<unsigned char*>(malloc(file_length));

	memcpy(sounds[sfx_num].buf, file_handle, file_length);

	sounds[sfx_num].length = file_length / 2;
	src = sounds[sfx_num].buf;
	dest = (unsigned short *)sounds[sfx_num].buf;
	for (i=0; i<sounds[sfx_num].length; i++)
	{
		unsigned short temp;
		temp = src[0] + (src[1] << 8);
		*dest = temp;
		src += 2;
		dest++;
	}
	return 0;
}

void dj_free_sfx(unsigned char sfx_num)
{
	free(sounds[sfx_num].buf);
	memset(&sounds[sfx_num], 0, sizeof(sfx_data));
}

/* mod handling */

char dj_ready_mod(unsigned char *datafile_buffer)
{
#ifndef NO_SDL_MIXER
	FILE *tmp;
	int tmp_fd;
	char* filename;
	unsigned char *fp;
	int len;

		fp = dat_open("bump.mod", datafile_buffer);
		len = dat_filelen("bump.mod", datafile_buffer);


	if (Mix_PlayingMusic())
		Mix_FadeOutMusic(1500);

	if (current_music) {
		Mix_FreeMusic(current_music);
		current_music = NULL;
	}

	if (fp == nullptr) {
		return 0;
	}

#ifdef _MSC_VER
	// FIXME: Implement safe temporary files for MSVC (needs alternative for mkstemp)
	filename = strdup("jnb.tmpmusic.mod");
	tmp = fopen(filename, "wb");
	if (tmp) {
		fwrite(fp, len, 1, tmp);
		fflush(tmp);
		fclose(tmp);
	}
#else
#ifdef __MINGW32__
	filename = strdup("jumpnbump.mod.XXXXXX");
#else // Unix
	filename = strdup("/tmp/jumpnbump.mod.XXXXXX");
#endif
	tmp_fd = mkstemp(filename);
	if (tmp_fd == -1) {
		free(filename);
		return 0;
	}
	tmp = fdopen(tmp_fd, "wb");
	if (!tmp) {
		free(filename);
		return 0;
	}
	fwrite(fp, len, 1, tmp);
	fflush(tmp);
	fclose(tmp);
#endif

	current_music = Mix_LoadMUS(filename);
	remove(filename);
	free(filename);
	if (current_music == NULL) {
		fprintf(stderr, "Couldn't load music: %s\n", SDL_GetError());
		return 0;
	}

#endif

	return 0;
}

char dj_start_mod()
{
#ifndef NO_SDL_MIXER

	Mix_VolumeMusic(0);
	Mix_PlayMusic(current_music, -1);
#endif

	return 0;
}

void dj_stop_mod()
{
#ifndef NO_SDL_MIXER

	Mix_HaltMusic();
#endif
}

void dj_set_mod_volume(char volume)
{
#ifndef NO_SDL_MIXER

	Mix_VolumeMusic(volume);
#endif
}