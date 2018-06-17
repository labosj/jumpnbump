//
// Created by edwin on 01-06-18.
//

#include "globals.h"
#include "data.h"
#include <fcntl.h>
#include <unistd.h>

[[deprecated]]
unsigned char *dat_open(const std::string &file_name, unsigned char *datafile_buffer) {
    int num;
    int c1;
    char name[21];
    int ofs;
    unsigned char *ptr;

    if (datafile_buffer == nullptr)
        return 0;

    memset(name, 0, sizeof(name));

    num = ((datafile_buffer[0] << 0) +
           (datafile_buffer[1] << 8) +
           (datafile_buffer[2] << 16) +
           (datafile_buffer[3] << 24));

    ptr = datafile_buffer + 4;

    for (c1 = 0; c1 < num; c1++) {

        memcpy(name, ptr, 12);
        ptr += 12;


        if (name == file_name) {
            ofs = ((ptr[0] << 0) +
                   (ptr[1] << 8) +
                   (ptr[2] << 16) +
                   (ptr[3] << 24));

            return (datafile_buffer + ofs);
        }
        ptr += 8;
    }

    return 0;
}

int dat_filelen(const std::string &file_name, unsigned char *datafile_buffer) {
    unsigned char *ptr;
    int num;
    int c1;
    char name[21];
    int len;

    memset(name, 0, sizeof(name));

    num = ((datafile_buffer[0] << 0) +
           (datafile_buffer[1] << 8) +
           (datafile_buffer[2] << 16) +
           (datafile_buffer[3] << 24));

    ptr = datafile_buffer + 4;

    for (c1 = 0; c1 < num; c1++) {

        memcpy(name, ptr, 12);
        ptr += 12;

        if (name == file_name) {

            ptr += 4;
            len = ((ptr[0] << 0) +
                   (ptr[1] << 8) +
                   (ptr[2] << 16) +
                   (ptr[3] << 24));

            return len;
        }
        ptr += 8;
    }

    return 0;
}

int filelength(int handle) {
    struct stat buf;

    if (fstat(handle, &buf) == -1) {
        perror("filelength");
        exit(EXIT_FAILURE);
    }

    return buf.st_size;
}

#ifndef O_BINARY
#define O_BINARY 0
#endif

void preread_datafile(const std::string& fname, unsigned char** datafile_buffer) {
    int fd = 0;
    int len;


    fd = open(fname.c_str(), O_RDONLY | O_BINARY);
    if (fd == -1) {
        fprintf(stderr, "can't open %s:", fname.c_str());
        perror("");
        exit(42);
    }

    len = filelength(fd);
    *datafile_buffer = (unsigned char *) malloc(len);
    if (*datafile_buffer == nullptr) {
        perror("malloc()");
        close(fd);
        exit(42);
    }

    if (read(fd, *datafile_buffer, len) != len) {
        perror("read()");
        close(fd);
        exit(42);
    }

    close(fd);
}
