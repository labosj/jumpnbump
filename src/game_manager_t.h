//
// Created by edwin on 13-06-18.
//

#ifndef JUMPNBUMP_GAME_MANAGER_T_H
#define JUMPNBUMP_GAME_MANAGER_T_H

#include <SFML/Audio.hpp>

class game_manager_t {
public:
    sf::Clock clock;
    int frame_counter = 0;

    void reset_frames();
    int get_elapsed_frames();
};


#endif //JUMPNBUMP_GAME_MANAGER_T_H
