//
// Created by edwin on 01-06-18.
//

#ifndef JUMPNBUMP_DATA_H
#define JUMPNBUMP_DATA_H


int filelength(int handle);

unsigned char *dat_open(const std::string &file_name, unsigned char *datafile_buffer);

int dat_filelen(const std::string & file_name, unsigned char *datafile_buffer);

void write_calib_data(void);

#include <src/main_info.h>

#endif //JUMPNBUMP_DATA_H