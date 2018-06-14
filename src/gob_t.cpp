//
// Created by edwin on 31-05-18.
//

#include "gob_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstring>

gob_t rabbit_gobs;
gob_t object_gobs;

int gob_t::add(unsigned char *handle, int len, sf::Texture& texture)
{

/*    sprite.setTextureRect(sf::IntRect(10, 10, 50, 30));
    sprite.setColor(sf::Color(255, 255, 255, 200));
    sprite.setPosition(100, 25);
    */
    unsigned char *gob_data;
    int i;

    gob_data = reinterpret_cast<unsigned char*>(malloc(len));
    memcpy(gob_data, handle, len);

    auto num_images = (short)((gob_data[0]) + (gob_data[1] << 8));

    for (i=0; i< num_images; i++) {

        auto offset = (gob_data[i*4+2]) + (gob_data[i*4+3] << 8) + (gob_data[i*4+4] << 16) + (gob_data[i*4+5] << 24);

        auto image = image_t{};
        image.width = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
        image.height = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
        image.hs_x = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;
        image.hs_y = (short)((gob_data[offset]) + (gob_data[offset+1] << 8)); offset += 2;

/*
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(image.width, image.height));
*/




        auto image_size = image.width * image.height;
        image.orig_data =  malloc(image_size);

        memcpy(image.orig_data, &gob_data[offset], image_size);

        image.data = (unsigned short *)image.orig_data;

        this->images.push_back(image);

    }
    free(gob_data);
    return 0;

}