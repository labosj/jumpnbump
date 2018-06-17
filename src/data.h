//
// Created by edwin on 01-06-18.
//

#ifndef JUMPNBUMP_DATA_H
#define JUMPNBUMP_DATA_H


int filelength(int handle);


[[deprecated]]
unsigned char *dat_open(const std::string &file_name, unsigned char *datafile_buffer);

[[deprecated]]
int dat_filelen(const std::string & file_name, unsigned char *datafile_buffer);

[[deprecated]]
void preread_datafile(const std::string& fname, unsigned char **datafile_buffer);

#endif //JUMPNBUMP_DATA_H
