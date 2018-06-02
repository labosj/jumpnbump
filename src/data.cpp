//
// Created by edwin on 01-06-18.
//

#include "globals.h"
#include "data.h"

unsigned char *dat_open(const std::string &file_name, unsigned char *datafile_buffer) {
    int num;
    int c1;
    char name[21];
    int ofs;
    unsigned char *ptr;

    if (datafile_buffer == NULL)
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

void write_calib_data(void) {
    FILE *handle;
    int c1;
    int len, num;
    char *mem;
    int ofs;

    if ((handle = fopen(datfile_name, "rb")) == nullptr)
        return;
    len = filelength(fileno(handle));
    if ((mem = reinterpret_cast<char *>(malloc(len))) == nullptr)
        return;
    fread(mem, 1, len, handle);
    fclose(handle);

    ofs = 4;
    num = *(int *) (&mem[0]);
    for (c1 = 0; c1 < num; c1++) {
        if (strnicmp(&mem[ofs], "calib.dat", strlen("calib.dat")) == 0) {
            ofs = *(int *) (&mem[ofs + 12]);
            break;
        }
        ofs += 20;
    }

    mem[ofs] = joy.calib_data.x1 & 0xff;
    mem[ofs + 1] = (joy.calib_data.x1 >> 8) & 0xff;
    mem[ofs + 2] = (joy.calib_data.x1 >> 16) & 0xff;
    mem[ofs + 3] = (joy.calib_data.x1 >> 24) & 0xff;
    mem[ofs + 4] = joy.calib_data.x2 & 0xff;
    mem[ofs + 5] = (joy.calib_data.x2 >> 8) & 0xff;
    mem[ofs + 6] = (joy.calib_data.x2 >> 16) & 0xff;
    mem[ofs + 7] = (joy.calib_data.x2 >> 24) & 0xff;
    mem[ofs + 8] = joy.calib_data.x3 & 0xff;
    mem[ofs + 9] = (joy.calib_data.x3 >> 8) & 0xff;
    mem[ofs + 10] = (joy.calib_data.x3 >> 16) & 0xff;
    mem[ofs + 11] = (joy.calib_data.x3 >> 24) & 0xff;
    mem[ofs + 12] = joy.calib_data.y1 & 0xff;
    mem[ofs + 13] = (joy.calib_data.y1 >> 8) & 0xff;
    mem[ofs + 14] = (joy.calib_data.y1 >> 16) & 0xff;
    mem[ofs + 15] = (joy.calib_data.y1 >> 24) & 0xff;
    mem[ofs + 16] = joy.calib_data.y2 & 0xff;
    mem[ofs + 17] = (joy.calib_data.y2 >> 8) & 0xff;
    mem[ofs + 18] = (joy.calib_data.y2 >> 16) & 0xff;
    mem[ofs + 19] = (joy.calib_data.y2 >> 24) & 0xff;
    mem[ofs + 20] = joy.calib_data.y3 & 0xff;
    mem[ofs + 21] = (joy.calib_data.y3 >> 8) & 0xff;
    mem[ofs + 22] = (joy.calib_data.y3 >> 16) & 0xff;
    mem[ofs + 23] = (joy.calib_data.y3 >> 24) & 0xff;

    if ((handle = fopen(datfile_name, "wb")) == NULL)
        return;
    fwrite(mem, 1, len, handle);
    fclose(handle);

}