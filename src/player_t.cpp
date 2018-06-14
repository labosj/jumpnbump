//
// Created by edwin on 01-06-18.
//

#include "globals.h"
#include "player_t.h"
#include "ban_map.h"
#include "anim_t.h"
#include "util.h"
#include <iostream>
#include "objects_t.h"
#include "sound_manager_t.h"

std::vector<player_t> players;

extern int jetpack;
extern int bunnies_in_space;
extern int pogostick;
extern int blood_is_thicker_than_water;
extern main_info_t main_info;

void player_t::set_position(const position_t &position) {
    this->position = position;
}

void serverSendKillPacket(int c1, int c2);

position_t player_t::get_position() const {
    return this->position;
}

void player_action_left(player_t &player) {

    screen_position_t pixel_pos = player.position;
    auto below_left = ban_map.get(pixel_pos + screen_position_t{0, 16});
    auto below = ban_map.get(pixel_pos + screen_position_t{8, 16});
    auto below_right = ban_map.get(pixel_pos + screen_position_t{15, 16});

    if (below == ban_map_t::Type::ICE) {
        if (player.x_add > 0)
            player.x_add -= 1024;
        else
            player.x_add -= 768;
    } else if ((below_left != ban_map_t::Type::SOLID && below_right == ban_map_t::Type::ICE) ||
               (below_left == ban_map_t::Type::ICE && below_right != ban_map_t::Type::SOLID)) {
        if (player.x_add > 0)
            player.x_add -= 1024;
        else
            player.x_add -= 768;
    } else {
        if (player.x_add > 0) {
            player.x_add -= 16384;
            if (player.x_add > -98304L && player.in_water == 0 && below == ban_map_t::Type::SOLID)
                objects.add(object_t::Type::SMOKE, player.get_position() + screen_position_t{2 + rnd(9), 13 + rnd(5)}, 0,
                           -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
        } else
            player.x_add -= 12288;
    }
    if (player.x_add < -98304L)
        player.x_add = -98304L;
    player.direction = 1;

    if (player.anim_handler.anim == 0) {
        player.set_anim(1);
    }
}

void player_action_right(player_t &player) {

    auto below_left = ban_map.get(player.get_position() + screen_position_t{0, 16});
    auto below = ban_map.get(player.get_position() + screen_position_t{8, 16});
    auto below_right = ban_map.get(player.get_position() + screen_position_t{15, 16});

    if (below == ban_map_t::Type::ICE) {
        if (player.x_add < 0)
            player.x_add += 1024;
        else
            player.x_add += 768;
    } else if ((below_left != ban_map_t::Type::SOLID && below_right == ban_map_t::Type::ICE) ||
               (below_left == ban_map_t::Type::ICE && below_right != ban_map_t::Type::SOLID)) {
        if (player.x_add > 0)
            player.x_add += 1024;
        else
            player.x_add += 768;
    } else {
        if (player.x_add < 0) {
            player.x_add += 16384;
            if (player.x_add < 98304L && player.in_water == 0 && below == ban_map_t::Type::SOLID)
                objects.add_smoke(player);
        } else
            player.x_add += 12288;
    }
    if (player.x_add > 98304L)
        player.x_add = 98304L;
    player.direction = 0;
    if ( player.anim_handler.anim == 0 )
        player.set_anim(1);
}

void player_no_action(player_t &player) {
    auto below_left = ban_map.get(player.get_position() + screen_position_t{0, 16});
    auto below = ban_map.get(player.get_position() + screen_position_t{8, 16});
    auto below_right = ban_map.get(player.get_position() + screen_position_t{15, 16});

    if (below == ban_map_t::Type::SOLID || below == ban_map_t::Type::SPRING ||
        (((below_left == ban_map_t::Type::SOLID || below_left == ban_map_t::Type::SPRING) &&
          below_right != ban_map_t::Type::ICE) ||
         (below_left != ban_map_t::Type::ICE &&
          (below_right == ban_map_t::Type::SOLID || below_right == ban_map_t::Type::SPRING)))) {
        if (player.x_add < 0) {
            player.x_add += 16384;
            if (player.x_add > 0)
                player.x_add = 0;
        } else {
            player.x_add -= 16384;
            if (player.x_add < 0)
                player.x_add = 0;
        }
        if (player.x_add != 0 && below == ban_map_t::Type::SOLID)
            objects.add_smoke(player);
    }
    if ( player.anim_handler.anim == 1 )
        player.set_anim(0);
}

void player_t::set_anim(int anim) {
    this->anim_handler.anim = anim;
    this->anim_handler.frame = 0;
    this->anim_handler.frame_tick = 0;
    this->anim_handler.image = player_anims[this->anim_handler.anim].frame[this->anim_handler.frame].image + this->direction * 9;
}

void player_t::gravity_fall() {

    auto gravity = 32768;
    if (bunnies_in_space == 0)
        gravity = 16384;

    this->jump_ready = 1;
    if (this->in_water == 0 && this->y_add < 0 && this->jump_abort == 1) {
        this->y_add += gravity;
        if (this->y_add > 0)
            this->y_add = 0;
    }
}

/**
 * This code is pure bullshit
 * First check if the bunny do s  spring jump and then makes the spring animation according the
 * need a refactor
 */
void player_t::check_spring_jump() {
    position_t position = this->get_position();


    if (ban_map.get(this->get_position() + screen_position_t{8, 15}) == ban_map_t::Type::SPRING ||
        ((ban_map.get(this->get_position() + screen_position_t{0, 15}) == ban_map_t::Type::SPRING &&
          ban_map.get(this->get_position() + screen_position_t{15, 15}) != ban_map_t::Type::SOLID) ||
         (ban_map.get(this->get_position() + screen_position_t{0, 15}) != ban_map_t::Type::SOLID &&
          ban_map.get(this->get_position() + screen_position_t{15, 15}) == ban_map_t::Type::SPRING))) {

        /**
         * this code just animate a spring
         * this can be simplified a lot
         */
        for (auto &object : objects.objects) {
            screen_position_t screen_position = position;
            if (object.used == 1 && object.type == object_t::Type::SPRING) {
                if (ban_map.get(screen_position + screen_position_t{8, 15}) == ban_map_t::Type::SPRING) {
                    if ((object.position.x >> 20) == ((screen_position.x + 8) >> 4) &&
                        (object.position.y >> 20) == ((screen_position.y + 15) >> 4)) {
                        object.set_anim(object.anim_handler.anim, 0);
                        break;
                    }
                } else {
                    if (ban_map.get(screen_position + screen_position_t{0, 15}) == ban_map_t::Type::SPRING) {
                        if ((object.position.x >> 20) == (screen_position.x >> 4) &&
                            (object.position.y >> 20) == ((screen_position.y + 15) >> 4)) {
                            object.set_anim(object.anim_handler.anim, 0);
                            break;
                        }
                    } else if (ban_map.get(screen_position + screen_position_t{15, 15}) == ban_map_t::Type::SPRING) {
                        if ((object.position.x >> 20) == ((screen_position.x + 15) >> 4) &&
                            (object.position.y >> 20) == ((screen_position.y + 15) >> 4)) {
                            object.set_anim(object.anim_handler.anim, 0);
                            break;
                        }
                    }
                }
            }
        }


        this->position.y = ((this->position.y >> 16) & 0xfff0) << 16;
        this->y_add = -400000L;
        this->set_anim(2);
        this->jump_ready = 0;
        this->jump_abort = 0;
        external_sound_manager->play_sfx_spring();
        /*dj_play_sfx(SFX_SPRING, (unsigned short) (SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, -1);*/
    }
}

void player_t::check_ceiling() {

    if (ban_map.get(this->get_position()) == ban_map_t::Type::SOLID ||
        ban_map.get(this->get_position()) == ban_map_t::Type::ICE ||
        ban_map.get(this->get_position()) == ban_map_t::Type::SPRING ||
        ban_map.get(this->get_position() + screen_position_t{15, 0}) == ban_map_t::Type::SOLID ||
        ban_map.get(this->get_position() + screen_position_t{15, 0}) == ban_map_t::Type::ICE ||
        ban_map.get(this->get_position() + screen_position_t{15, 0}) == ban_map_t::Type::SPRING) {
        //stop the velocity in y
        this->position.y = (((screen_position_t{this->get_position()}.y + 16) & 0xfff0)) << 16; //TODO: MASK
        this->y_add = 0;
        this->set_anim(0);
    }
}

void steer_players() {

    update_player_actions();

    for (auto& player : players) {
            if (player.is_alive()) {

                if (player.action_left && player.action_right) {
                    if (player.direction == 0) {
                        if (player.action_right) {
                            player_action_right(player);
                        }
                    } else {
                        if (player.action_left) {
                            player_action_left(player);
                        }
                    }
                } else if (player.action_left) {
                    player_action_left(player);
                } else if (player.action_right) {
                    player_action_right(player);
                } else if ((!player.action_left) && (!player.action_right)) {
                    player_no_action(player);

                }
                if (jetpack == 0) {
                    /* no jetpack */
                    if (pogostick == 1 || (player.jump_ready == 1 && player.action_up)) {

                        auto below_left = ban_map.get(player.get_position() + screen_position_t{0, 16});
                        auto below_right = ban_map.get(player.get_position() + screen_position_t{15, 16});

                        /* jump */
                        if (below_left == ban_map_t::Type::SOLID ||
                            below_left == ban_map_t::Type::ICE ||
                            below_right == ban_map_t::Type::SOLID ||
                            below_right == ban_map_t::Type::ICE) {
                            player.y_add = -280000L;
                            player.set_anim(2);
                            player.jump_ready = 0;
                            player.jump_abort = 1;
                            if (pogostick == 0) {
                                external_sound_manager->play_sfx_jump();
                                /*
                                    dj_play_sfx(SFX_JUMP, (unsigned short) (SFX_JUMP_FREQ + rnd(2000) - 1000),
                                                64, 0, -1);
                                                */
                            } else {
                                //external_sound_manager->play_sfx(SFX_SPRING);
                                external_sound_manager->play_sfx_spring();

                             /*   dj_play_sfx(SFX_SPRING,
                                            (unsigned short) (SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, -1);*/
                            }
                        }
                        /* jump out of water */
                        if (ban_map.is_in_water(player.get_position())) {
                            player.y_add = -196608L;
                            player.in_water = 0;
                            player.set_anim(2);
                            player.jump_ready = 0;
                            player.jump_abort = 1;
                            if (pogostick == 0) {
                                external_sound_manager->play_sfx_jump();

                            } else {
                                external_sound_manager->play_sfx_spring();
                            }

                        }
                    }
                    /* fall down by gravity */
                    if (pogostick == 0 && (!player.action_up)) {
                        player.gravity_fall();
                    }
                } else {
                    /* with jetpack */
                    if (player.action_up) {
                        player.y_add -= 16384;
                        if (player.y_add < -400000L)
                            player.y_add = -400000L;
                        if (ban_map.is_in_water(player.get_position()))
                            player.in_water = 0;
                        if (rnd(100) < 50)
                            objects.add_jetpack_smoke(player);
                    }
                }

                player.position.x += player.x_add;
                if ((player.position.x >> 16) < 0) {
                    player.position.x = 0;
                    player.x_add = 0;
                }
                if ((player.position.x >> 16) + 15 > 351) {
                    player.position.x = 336L << 16;
                    player.x_add = 0;
                }
                player.check_lateral_walls();

                player.position.y += player.y_add;

                player.check_spring_jump();

                player.check_ceiling();

                screen_position_t screen_position = player.get_position();
                if (ban_map.get(screen_position + screen_position_t{8, 8}) == ban_map_t::Type::WATER) {
                    if (player.in_water == 0) {
                        /* falling into water */
                        player.in_water = 1;
                        player.set_anim(4);


                        if (player.y_add >= 32768) {
                            screen_position_t screen_position = player.get_position();
                            screen_position.y &= 0xfff0;
                            objects.add(object_t::Type::SPLASH,
                                       screen_position
                                       + screen_position_t{9, 15}, 0, 0,
                                       OBJ_ANIM_SPLASH, 0);
                            external_sound_manager->play_sfx_splash();
                        }
                    }
                    /* slowly move up to water surface */
                    player.y_add -= 1536;
                    if (player.y_add < 0 && player.anim_handler.anim != 5) {
                        player.set_anim(5);
                    }
                    if (player.y_add < -65536L)
                        player.y_add = -65536L;
                    if (player.y_add > 65535L)
                        player.y_add = 65535L;

                    if (ban_map.get(screen_position + screen_position_t{0, 15}) == ban_map_t::Type::SOLID ||
                        ban_map.get(screen_position + screen_position_t{0, 15}) == ban_map_t::Type::ICE ||
                        ban_map.get(screen_position + screen_position_t{15, 15}) == ban_map_t::Type::SOLID ||
                        ban_map.get(screen_position + screen_position_t{15, 15}) == ban_map_t::Type::ICE) {
                        player.position.y = (((screen_position.y + 16) & 0xfff0) - 16) << 16;
                        player.y_add = 0;
                    }
                } else if (ban_map.get(player.get_position() + screen_position_t{0, 15}) == ban_map_t::Type::SOLID ||
                           ban_map.get(player.get_position() + screen_position_t{0, 15}) == ban_map_t::Type::ICE ||
                           ban_map.get(player.get_position() + screen_position_t{0, 15}) == ban_map_t::Type::SPRING ||
                           ban_map.get(player.get_position() + screen_position_t{15, 15}) == ban_map_t::Type::SOLID ||
                           ban_map.get(player.get_position() + screen_position_t{15, 15}) == ban_map_t::Type::ICE ||
                           ban_map.get(player.get_position() + screen_position_t{15, 15}) == ban_map_t::Type::SPRING) {
                    player.in_water = 0;
                    player.position.y = (((screen_position.y + 16) & 0xfff0) - 16) << 16;
                    player.y_add = 0;
                    if (player.anim_handler.anim != 0 && player.anim_handler.anim != 1) {
                        player.set_anim(0);
                    }
                } else {
                    if (player.in_water == 0) {
                        if (bunnies_in_space == 0)
                            player.y_add += 12288;
                        else
                            player.y_add += 6144;
                        if (player.y_add > 327680L)
                            player.y_add = 327680L;
                    } else {
                        player.position.y = (player.position.y & 0xffff0000) + 0x10000;
                        player.y_add = 0;
                    }
                    player.in_water = 0;
                }
                if (player.y_add > 36864 && player.anim_handler.anim != 3 && player.in_water == 0) {
                    player.set_anim(3);
                }

            }

            player.anim_handler.frame_tick++;
            if (player.anim_handler.frame_tick >= player_anims[player.anim_handler.anim].frame[player.anim_handler.frame].ticks) {
                player.anim_handler.frame++;
                if (player.anim_handler.frame >= player_anims[player.anim_handler.anim].frame.size()) {
                    if (player.anim_handler.anim != 6)
                        player.anim_handler.frame = player_anims[player.anim_handler.anim].restart_frame;
                    else position_player(player);
                }
                player.anim_handler.frame_tick = 0;
            }
            player.anim_handler.image = player_anims[player.anim_handler.anim].frame[player.anim_handler.frame].image + player.direction * 9;

        }


}

void player_t::check_lateral_walls() {

    if (ban_map.get(this->get_position()) == ban_map_t::Type::SOLID ||
        ban_map.get(this->get_position()) == ban_map_t::Type::ICE ||
        ban_map.get(this->get_position()) == ban_map_t::Type::SPRING ||
        ban_map.get(this->get_position() + screen_position_t{0, 15}) == ban_map_t::Type::SOLID ||
        ban_map.get(this->get_position() + screen_position_t{0, 15}) == ban_map_t::Type::ICE ||
        ban_map.get(this->get_position() + screen_position_t{0, 15}) == ban_map_t::Type::SPRING) {
        int s1 = (this->position.x >> 16);
        this->position.x = (((s1 + 16) & 0xfff0)) << 16;
        this->x_add = 0;
    }


    if (ban_map.get(this->get_position() + screen_position_t{15, 0}) == ban_map_t::Type::SOLID ||
        ban_map.get(this->get_position() + screen_position_t{15, 0}) == ban_map_t::Type::ICE ||
        ban_map.get(this->get_position() + screen_position_t{15, 0}) == ban_map_t::Type::SPRING ||
        ban_map.get(this->get_position() + screen_position_t{15, 15}) == ban_map_t::Type::SOLID ||
        ban_map.get(this->get_position() + screen_position_t{15, 15}) == ban_map_t::Type::ICE ||
        ban_map.get(this->get_position() + screen_position_t{15, 15}) == ban_map_t::Type::SPRING) {
        int s1 = (this->position.x >> 16);
        this->position.x = (((s1 + 16) & 0xfff0) - 16) << 16;
        this->x_add = 0;
    }
}

void position_player(player_t &player) {
    map_position_t position;


    again:

        position = ban_map.get_random_available_floor_position();

        //verifica que el conejo no este cerca de otros conejos
        for (const auto &other_player : players) {
            if (other_player.get_id() != player.get_id()) {
                screen_position_t screen_position = position;
                if (abs(screen_position.x - (screen_position_t{other_player.get_position()}.x)) < 32 &&
                    abs(screen_position.y - (screen_position_t{other_player.get_position()}.y)) < 32)
                    goto again;
            }
        }

    player.set_position(position);

    player.x_add = player.y_add = 0;
    player.direction = 0;
    player.jump_ready = 1;
    player.in_water = 0;
    player.anim_handler.anim = 0;
    player.anim_handler.frame = 0;
    player.anim_handler.frame_tick = 0;
    player.anim_handler.image = player_anims[player.anim_handler.anim].frame[player.anim_handler.frame].image;

    player.dead_flag = 0;

}

void player_kill(player_t &player_1, player_t &player_2) {

    if (player_1.y_add >= 0) {

        serverSendKillPacket(player_1.get_id(), player_2.get_id());
    } else {
        if (player_2.y_add < 0)
            player_2.y_add = 0;
    }
}

void check_collision(player_t &player_1, player_t &player_2) {

        if (labs(player_1.position.x - player_2.position.x) < (12L << 16) &&
            labs(player_1.position.y - player_2.position.y) < (12L << 16)) {
            if ((labs(player_1.position.y - player_2.position.y) >> 16) > 5) {
                if (player_1.position.y < player_2.position.y) {
                    player_kill(player_1, player_2);
                } else {
                    player_kill(player_2, player_1);
                }
            } else {
                if (player_1.position.x < player_2.position.x) {
                    if (player_1.x_add > 0)
                        player_1.position.x = player_2.position.x - (12L << 16);
                    else if (player_2.x_add < 0)
                        player_2.position.x = player_1.position.x + (12L << 16);
                    else {
                        player_1.position.x -= player_1.x_add;
                        player_2.position.x -= player_2.x_add;
                    }
                    int l1 = player_2.x_add;
                    player_2.x_add = player_1.x_add;
                    player_1.x_add = l1;
                    if (player_1.x_add > 0)
                        player_1.x_add = -player_1.x_add;
                    if (player_2.x_add < 0)
                        player_2.x_add = -player_2.x_add;
                } else {
                    if (player_1.x_add > 0)
                        player_2.position.x = player_1.position.x - (12L << 16);
                    else if (player_2.x_add < 0)
                        player_1.position.x = player_2.position.x + (12L << 16);
                    else {
                        player_1.position.x -= player_1.x_add;
                        player_2.position.x -= player_2.x_add;
                    }
                    int l1 = player_2.x_add;
                    player_2.x_add = player_1.x_add;
                    player_1.x_add = l1;
                    if (player_1.x_add < 0)
                        player_1.x_add = -player_1.x_add;
                    if (player_2.x_add > 0)
                        player_2.x_add = -player_2.x_add;
                }
            }
        }
}

void collision_check() {

    for (auto i = 0; i < players.size(); i++) {
        for (auto j = i + 1; j < players.size(); j++) {
            check_collision(players[i], players[j]);

        }
    }
}

void player_t::reset_kills() {
    this->bumps = 0;
    this->bumped = std::vector<int>(players.size(), 0);
}

void init_players()
{

    players.clear();
    for (auto c1 = 0; c1 < 4; c1++) {
        //create bunnies randomly in the menu screen
        auto player = player_t{c1};
        player.position = screen_position_t{rnd(150), (160L + c1 * 2)};
        player.x_add = 0;
        player.y_add = 0;
        player.direction = rnd(2);
        player.jump_ready = 1;
        player.anim_handler.anim = 0;
        player.anim_handler.frame = 0;
        player.anim_handler.frame_tick = 0;
        player.anim_handler.image = player_anims[player.anim_handler.anim].frame[player.anim_handler.frame].image;

        players.push_back(player);
    }
}