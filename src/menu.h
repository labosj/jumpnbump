//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_MENU_H
#define JUMPNBUMP_MENU_H

struct main_info_t;
struct leftovers_t;

int menu(main_info_t& main_info, unsigned char* datafile_buffer, leftovers_t& leftovers);
int menu_init(main_info_t& main_info, unsigned char* datafile_buffer);
void menu_deinit(main_info_t& main_info);


#endif //JUMPNBUMP_MENU_H