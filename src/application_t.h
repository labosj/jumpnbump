//
// Created by edwin on 17-06-18.
//

#ifndef JUMPNBUMP_APPLICATION_T_H
#define JUMPNBUMP_APPLICATION_T_H

#include <SFML/Graphics/RenderWindow.hpp>

/**
 * Applicatin class
 *
 * This class holds a application instance
 */
class application_t {
private:
    sf::RenderWindow window;
public:

    sf::RenderWindow& get_window() { return this->window; }
    bool init();
};


#endif //JUMPNBUMP_APPLICATION_T_H
