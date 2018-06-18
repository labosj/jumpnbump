//
// Created by edwin on 13-06-18.
//

#include "game_manager_t.h"
#include "gob_t.h"
#include "leftovers_t.h"
#include "player_t.h"
#include "anim_t.h"
#include "globals.h"
#include "ban_map_t.h"
#include <SFML/Window.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "objects_t.h"
#include "util.h"
#include "sound_manager_t.h"

game_manager_t::game_manager_t(sf::RenderWindow& window) :window(window) {
}

void game_manager_t::init_textures() {

    this->object_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/objects.png");
    this->rabbit_texture.loadFromFile("/home/edwin/Projects/jumpnbump/data/rabbit.png");

}

void game_manager_t::init_deprecated_data() {
    this->player_anims = {
            {0, {{ 0, 0x7fff}}},
            {0, {{0, 4}, {1, 4}, {2, 4}, {3, 4}}},
            {0, {{4, 0x7fff}}},
            {2, {{5, 8}, {6, 10}, {7, 3}, {6, 3}}},
            {0, {{6, 0x7fff}}},
            {1, {{5, 8}, {4, 0x7fff}}},
            {0, {{8, 5}}}
    };


    this->object_anims = {
            {
                    0,
                    {
                            {0,  3},
                            {1,  3},
                            {2,  3},
                            {3,  3},
                            {4,  3},
                            {5,  3}
                    }
            },
            {
                    0,
                    {
                            {6,  2},
                            {7,  2},
                            {8,  2},
                            {9,  2},
                            {10, 2},
                            {11, 2},
                            {12, 2},
                            {13, 2},
                            {14, 2}
                    }
            },
            {
                    0,
                    {
                            {15, 3},
                            {16, 3},
                            {16, 3},
                            {17, 3},
                            {18, 3},
                            {19, 3}
                    }
            },
            {
                    0,
                    {
                            {20, 2},
                            {21, 2},
                            {22, 2},
                            {23, 2},
                            {24, 2},
                            {25, 2},
                            {24, 2},
                            {23, 2},
                            {22, 2},
                            {21, 2}
                    }
            },
            {
                    0,
                    {
                            {26, 2},
                            {27, 2},
                            {28, 2},
                            {29, 2},
                            {30, 2},
                            {31, 2},
                            {30, 2},
                            {29, 2},
                            {28, 2},
                            {27, 2}
                    }
            },
            {
                    0,
                    {
                            {32, 2},
                            {33, 2},
                            {34, 2},
                            {35, 2},
                            {36, 2},
                            {37, 2},
                            {36, 2},
                            {35, 2},
                            {34, 2},
                            {33, 2}
                    }
            },
            {
                    0,
                    {
                            {38, 2},
                            {39, 2},
                            {40, 2},
                            {41, 2},
                            {42, 2},
                            {43, 2},
                            {42, 2},
                            {41, 2},
                            {40, 2},
                            {39, 2}
                    }
            },
            {
                    0,
                    {
                            {76, 4},
                            {77, 4},
                            {78, 4},
                            {79, 4}
                    }
            }
    };

    rabbit_gobs.add("/home/edwin/Projects/jumpnbump/data/rabbit.json", this->rabbit_texture);


    object_gobs.add("/home/edwin/Projects/jumpnbump/data/objects.json", this->object_texture);

}

void game_manager_t::draw() {
    this->window.clear();

    this->stage.draw_background(this->window);


    for (int i = 0 ; i < players.size(); i++) {
        this->pobs.add(players[i].get_position(), players[i].anim_handler.image + i * 18, &rabbit_gobs);
    }

    this->pobs.draw(*this);
    leftovers.draw(*this);

    this->stage.draw_foreground(this->window);

    this->window.display();

}

void game_manager_t::draw_pob(const pob_t& pob)
{
    auto& image = pob.pob_data->images[pob.image];
    image.setPosition(pob.position.x, pob.position.y);
    this->window.draw(image);

}

void game_manager_t::reset_frames() {
    this->clock.restart();
    this->frame_counter= 0;
}

int game_manager_t::get_elapsed_frames() {
    auto elapsed = this->clock.getElapsedTime();

    const auto milliseconds_per_frame = 1000 / 60;

    auto frames = elapsed.asMilliseconds() / milliseconds_per_frame;;
    if ( this->frame_counter < frames ) {
        auto elapsed_frames = frames - this->frame_counter;
        this->frame_counter = frames;
        return elapsed_frames;
    } else {
        return 0;
    }

}

void game_manager_t::process_input() {
    sf::Event event;
    while (this->window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            this->window.close();
        if ( event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased ) {
            if ( event.key.code == sf::Keyboard::Key::Escape ) {
                this->window.close();
            } else {
                for ( auto& player : players) {
                    player.control.update(event);
                }
            }
        }
    }
}

bool game_manager_t::init() {

        this->stage.load("/home/edwin/Projects/jumpnbump/data/maps/default");
        this->init_textures();
        this->init_deprecated_data();
        this->init_players();

        auto& ban_map = this->stage.get_map();

        for (auto& player : this->players) {
            player.reset_kills();
            player.position_player();
        }

        for (int c1 = 0; c1 < ban_map.get_height(); c1++) {
            for (int c2 = 0; c2 < ban_map.get_width() ; c2++) {
                if (ban_map.get(map_position_t{c2, c1}) == ban_map_t::Type::SPRING)
                    objects.add(*this, object_t::Type::SPRING, map_position_t{c2, c1}, 0, 0, OBJ_ANIM_SPRING, 5);
            }
        }

        for ( int i = 0 ; i < 2 ; i++ ) {
            auto new_pos = ban_map.get_random_available_position();
            objects.add(*this, object_t::Type::YEL_BUTFLY, screen_position_t{8, 8} + new_pos , (rnd(65535) - 32768) * 2,
                        (rnd(65535) - 32768) * 2,
                        0, 0);
        }

        for ( int i = 0 ; i < 2 ; i++ ) {
            auto new_pos = ban_map.get_random_available_position();
            objects.add(*this, object_t::Type::PINK_BUTFLY,  screen_position_t{8, 8} + new_pos, (rnd(65535) - 32768) * 2,
                        (rnd(65535) - 32768) * 2, 0, 0);
        }


        return true;

}

stage_t& game_manager_t::get_stage() {
    return this->stage;
}

void game_manager_t::loop() {


    this->init();

    this->sound_manager.load_sfx();
    this->stage.play_bgm();


    int update_count = 1;

    this->reset_frames();

    this->process_input();

    while (this->window.isOpen()) {
        while (update_count) {

            steer_players(*this);



            this->collision_check();


            objects.update(*this);


            if (update_count == 1) {
                this->draw();




            }

            update_count--;
        }

        this->process_input();
        update_count = this->get_elapsed_frames();

    }
}

void game_manager_t::init_players()
{

    this->players.clear();
    for (auto c1 = 0; c1 < 4; c1++) {
        //create bunnies randomly in the menu screen
        auto player = player_t{*this, c1};
        player.position = screen_position_t{rnd(150), (160 + c1 * 2)};
        player.x_add = 0;
        player.y_add = 0;
        player.direction = rnd(2);
        player.jump_ready = 1;
        player.anim_handler.anim = 0;
        player.anim_handler.frame = 0;
        player.anim_handler.frame_tick = 0;
        player.anim_handler.image = player_anims[player.anim_handler.anim].frame[player.anim_handler.frame].image;

        this->players.push_back(player);
    }

    {
        auto& control = this->players[0].control;
        control.up_key = sf::Keyboard::Key::W;
        control.left_key = sf::Keyboard::Key::A;
        control.right_key = sf::Keyboard::Key::D;
    }

    {
        auto& control = this->players[1].control;
        control.up_key = sf::Keyboard::Key::I;
        control.left_key = sf::Keyboard::Key::J;
        control.right_key = sf::Keyboard::Key::L;
    }

    {
        auto& control = this->players[2].control;
        control.up_key = sf::Keyboard::Key::Up;
        control.left_key = sf::Keyboard::Key::Left;
        control.right_key = sf::Keyboard::Key::Right;
    }

    {
        auto& control = this->players[3].control;
        control.up_key = sf::Keyboard::Key::Numpad8;
        control.left_key = sf::Keyboard::Key::Numpad4;
        control.right_key = sf::Keyboard::Key::Numpad6;
    }
}

void game_manager_t::collision_check() {

    for (auto i = 0; i < this->players.size(); i++) {
        for (auto j = i + 1; j < this->players.size(); j++) {
            check_collision(this->players[i], this->players[j]);

        }
    }
}

void game_manager_t::kill(int killer, int victim) {
    int c1 = killer;
    int c2 = victim;
    int c4 = 0;

    auto& players = this->players;

    players[c1].y_add = -players[c1].y_add;
    if (players[c1].y_add > -262144L)
        players[c1].y_add = -262144L;
    players[c1].jump_abort = 1;
    players[c2].dead_flag = 1;
    if (players[c2].anim_handler.anim != 6) {
        players[c2].set_anim(6);
        if (this->gore ) {
            auto screen_position = screen_position_t{players[victim].get_position()} + screen_position_t{6 + rnd(5), 6 + rnd(5)};
            for (c4 = 0; c4 < 6; c4++)
                objects.add(*this, object_t::Type::FUR, screen_position, (rnd(65535) - 32768) * 3,
                            (rnd(65535) - 32768) * 3, 0, 44 + c2 * 8);
            for (c4 = 0; c4 < 6; c4++)
                objects.add(*this, object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                            (rnd(65535) - 32768) * 3, 0, 76);
            for (c4 = 0; c4 < 6; c4++)
                objects.add(*this, object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                            (rnd(65535) - 32768) * 3, 0, 77);
            for (c4 = 0; c4 < 8; c4++)
                objects.add(*this, object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                            (rnd(65535) - 32768) * 3, 0, 78);
            for (c4 = 0; c4 < 10; c4++)
                objects.add(*this, object_t::Type::FLESH, screen_position, (rnd(65535) - 32768) * 3,
                            (rnd(65535) - 32768) * 3, 0, 79);
        }
        this->sound_manager.play_sfx_death();


        players[c1].count_kill(players[c2]);
    }
}