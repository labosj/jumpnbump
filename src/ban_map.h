//
// Created by edwin on 02-06-18.
//

#ifndef JUMPNBUMP_BAN_MAP_H
#define JUMPNBUMP_BAN_MAP_H

#include <tuple>

const unsigned int BAN_VOID	= 0;
const unsigned int BAN_SOLID = 1;
const unsigned int BAN_WATER = 2;
const unsigned int BAN_ICE = 3;
const unsigned int BAN_SPRING =	4;

class ban_map_t {

public:
    static const int WIDTH = 22;
    static const int HEIGHT = 17;

public:
    /**
    * This is used for objects
    * @param x
    * @param y
    * @return
    */
    unsigned int& get_xy(int x, int y);
    unsigned int& get(std::pair<int, int> position);
    const unsigned int& get(std::pair<int, int> position) const;
    const unsigned int& get_xy(int x, int y) const;
    bool is_water(int x, int y) const;
    std::pair<int, int> get_random_position() const;

    /**
     * Jusdt return a position with no BAN_VOID map
     * @return
     */
    std::pair<int, int> get_random_available_position() const;

    /**
     * Return a position suitable for a bunny new bunny in the floor
     * @return
     */
    std::pair<int, int> get_random_available_floor_position() const;

    unsigned int map[ban_map_t::HEIGHT][ban_map_t::WIDTH] = {
            {1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0},
            {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {1, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
            {1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 3, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1},
            {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
            {1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1},
            {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 1, 1},
            {2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
    };
};

/**
 * para llamar a x e y usar ban_map[y][x], notar el cambio de order de x e y
 */
[[deprecated]]
extern unsigned int (&ban_map)[ban_map_t::HEIGHT][ban_map_t::WIDTH];

/**
 * This must replace completely ban_map.
 * Just keep for migration propuses
 */
extern ban_map_t ban_map_new;


#endif //JUMPNBUMP_BAN_MAP_H
