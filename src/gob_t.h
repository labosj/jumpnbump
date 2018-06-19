//
// Created by edwin on 31-05-18.
//

#ifndef JUMPNBUMP_GOB_H
#define JUMPNBUMP_GOB_H

#include <vector>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

/**
 * Maybe this stands for graphic object
 *

 */
struct gob_t{
	std::vector<sf::Sprite> images;

	int add(const std::string& filename, sf::Texture& texture);

	sf::Sprite& get_sprite(int image);
};

#endif //JUMPNBUMP_GOB_H
