//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_BAN_MAP_H
#define JUMPNBUMP_BAN_MAP_H

const unsigned int BAN_VOID	= 0;
const unsigned int BAN_SOLID = 1;
const unsigned int BAN_WATER = 2;
const unsigned int BAN_ICE = 3;
const unsigned int BAN_SPRING =	4;

extern unsigned int ban_map[17][22];

unsigned int& GET_BAN_MAP_XY(int x, int y);
bool GET_BAN_MAP_IN_WATER(int x,int y);

#endif //JUMPNBUMP_BAN_MAP_H
