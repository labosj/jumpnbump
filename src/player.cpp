//
// Created by edwin on 01-06-18.
//

#include "globals.h"
#include "player.h"
#include "ban_map.h"
#include "object_anim.h"
#include "util.h"

extern int jetpack;
extern int bunnies_in_space;
extern int pogostick;
extern int blood_is_thicker_than_water;
extern main_info_t main_info;
extern object_anim_t object_anims[8];

void player_t::set_position(const position_t& position) {
    this->position.x = position.x;
    this->position.y = position.y;
}

void serverSendKillPacket(int c1, int c2);

position_t player_t::get_position() const {
    return this->position;
}

void player_action_left(player_t& player) {

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
                add_object(OBJ_SMOKE, player.position.to_pixels() + screen_position_t{2 + rnd(9), 13 + rnd(5)}, 0,
                           -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
        } else
            player.x_add -= 12288;
    }
    if (player.x_add < -98304L)
        player.x_add = -98304L;
    player.direction = 1;
    if (player.anim == 0) {
        player.anim = 1;
        player.frame = 0;
        player.frame_tick = 0;
        player.image = player_anims[player.anim].frame[player.frame].image + player.direction * 9;
    }
}

void player_action_right(player_t& player) {

    auto screen_position = screen_position_t{player.get_position()};
    auto below_left = ban_map.get(screen_position + screen_position_t{0, 16});
    auto below = ban_map.get(screen_position + screen_position_t{8, 16});
    auto below_right = ban_map.get(screen_position + screen_position_t{15, 16});

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
                add_smoke(player);
        } else
            player.x_add += 12288;
    }
    if (player.x_add > 98304L)
        player.x_add = 98304L;
    player.direction = 0;
    if (player.anim == 0) {
        player.anim = 1;
        player.frame = 0;
        player.frame_tick = 0;
        player.image = player_anims[player.anim].frame[player.frame].image + player.direction * 9;
    }
}

void steer_players() {
    int c1;
    int s1 = 0, s2 = 0;

    update_player_actions();

    for (c1 = 0; c1 < players.size(); c1++) {
        auto& player = players[c1];
        if (player.enabled == 1) {

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

                    s1 = (player.x >> 16);
                    s2 = (player.y >> 16);

                    screen_position_t screen_position = player.get_position();
                    auto below_left = ban_map.get(screen_position + screen_position_t{0, 16});
                    auto below = ban_map.get(screen_position + screen_position_t{8, 16});
                    auto below_right = ban_map.get(screen_position + screen_position_t{15, 16});
                    if (below == ban_map_t::Type::SOLID || below == ban_map_t::Type::SPRING ||
                        (((below_left == ban_map_t::Type::SOLID || below_left == ban_map_t::Type::SPRING) && below_right != ban_map_t::Type::ICE) ||
                         (below_left != ban_map_t::Type::ICE && (below_right == ban_map_t::Type::SOLID || below_right == ban_map_t::Type::SPRING)))) {
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
                            add_smoke(player);
                    }
                    if (player.anim == 1) {
                        player.anim = 0;
                        player.frame = 0;
                        player.frame_tick = 0;
                        player.image =
                                player_anims[player.anim].frame[player.frame].image + player.direction * 9;
                    }
                }
                if (jetpack == 0) {
                    /* no jetpack */
                    if (pogostick == 1 || (player.jump_ready == 1 && player.action_up)) {
                        s1 = (player.position.x >> 16);
                        s2 = (player.position.y >> 16);

                        screen_position_t screen_position = player.get_position();
                        auto below_left = ban_map.get(screen_position + screen_position_t{0, 16});
                        auto below = ban_map.get(screen_position + screen_position_t{8, 16});
                        auto below_right = ban_map.get(screen_position + screen_position_t{15, 16});

                        /* jump */
                        if (below_left == ban_map_t::Type::SOLID ||
                                below_left == ban_map_t::Type::ICE ||
                                below_right == ban_map_t::Type::SOLID ||
                                below_right == ban_map_t::Type::ICE) {
                            player.y_add = -280000L;
                            player.anim = 2;
                            player.frame = 0;
                            player.frame_tick = 0;
                            player.image = player_anims[player.anim].frame[player.frame].image +
                                               player.direction * 9;
                            player.jump_ready = 0;
                            player.jump_abort = 1;
                            if (pogostick == 0)
                                dj_play_sfx(main_info, SFX_JUMP, (unsigned short) (SFX_JUMP_FREQ + rnd(2000) - 1000),
                                            64, 0, -1);
                            else
                                dj_play_sfx(main_info, SFX_SPRING,
                                            (unsigned short) (SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, -1);
                        }
                        /* jump out of water */
                        if (ban_map.is_pixel_in_water(s1, s2)) {
                            player.y_add = -196608L;
                            player.in_water = 0;
                            player.anim = 2;
                            player.frame = 0;
                            player.frame_tick = 0;
                            player.image = player_anims[player.anim].frame[player.frame].image +
                                               player.direction * 9;
                            player.jump_ready = 0;
                            player.jump_abort = 1;
                            if (pogostick == 0)
                                dj_play_sfx(main_info, SFX_JUMP, (unsigned short) (SFX_JUMP_FREQ + rnd(2000) - 1000),
                                            64, 0, -1);
                            else
                                dj_play_sfx(main_info, SFX_SPRING,
                                            (unsigned short) (SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, -1);
                        }
                    }
                    /* fall down by gravity */
                    if (pogostick == 0 && (!player.action_up)) {
                        player.jump_ready = 1;
                        if (player.in_water == 0 && player.y_add < 0 && player.jump_abort == 1) {
                            if (bunnies_in_space == 0)
                                /* normal gravity */
                                player.y_add += 32768;
                            else
                                /* light gravity */
                                player.y_add += 16384;
                            if (player.y_add > 0)
                                player.y_add = 0;
                        }
                    }
                } else {
                    /* with jetpack */
                    if (player.action_up) {
                        player.y_add -= 16384;
                        if (player.y_add < -400000L)
                            player.y_add = -400000L;
                        if (ban_map.is_pixel_in_water(s1, s2))
                            player.in_water = 0;
                        if (rnd(100) < 50)
                            add_jetpack_smoke(player);
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
                {
                    if (player.position.y > 0) {
                        s2 = (player.position.y >> 16);
                    } else {
                        /* check top line only */
                        s2 = 0;
                    }

                    s1 = (player.position.x >> 16);
                    if (ban_map.get(screen_position_t{s1, s2}) == ban_map_t::Type::SOLID || ban_map.get(screen_position_t{s1, s2}) == ban_map_t::Type::ICE ||
                            ban_map.get(screen_position_t{s1, s2}) == ban_map_t::Type::SPRING || ban_map.get(screen_position_t{s1, (s2 + 15)}) == ban_map_t::Type::SOLID ||
                            ban_map.get(screen_position_t{s1, (s2 + 15)}) == ban_map_t::Type::ICE ||
                            ban_map.get(screen_position_t{s1, (s2 + 15)}) == ban_map_t::Type::SPRING) {
                        player.position.x = (((s1 + 16) & 0xfff0)) << 16;
                        player.x_add = 0;
                    }

                    s1 = (player.position.x >> 16);
                    if (ban_map.get(screen_position_t{(s1 + 15), s2}) == ban_map_t::Type::SOLID ||
                            ban_map.get(screen_position_t{(s1 + 15), s2}) == ban_map_t::Type::ICE ||
                            ban_map.get(screen_position_t{(s1 + 15), s2}) == ban_map_t::Type::SPRING ||
                            ban_map.get(screen_position_t{(s1 + 15), (s2 + 15)}) == ban_map_t::Type::SOLID ||
                            ban_map.get(screen_position_t{(s1 + 15), (s2 + 15)}) == ban_map_t::Type::ICE ||
                            ban_map.get(screen_position_t{(s1 + 15), (s2 + 15)}) == ban_map_t::Type::SPRING) {
                        player.position.x = (((s1 + 16) & 0xfff0) - 16) << 16;
                        player.x_add = 0;
                    }
                }

                player.position.y += player.y_add;

                s1 = (player.x >> 16);
                s2 = (player.y >> 16);
                if (s2 < 0)
                    s2 = 0;
                if (ban_map.get(screen_position_t{(s1 + 8), (s2 + 15)}) == ban_map_t::Type::SPRING ||
                    ((ban_map.get(screen_position_t{s1, (s2 + 15)}) == ban_map_t::Type::SPRING &&
                            ban_map.get(screen_position_t{(s1 + 15), (s2 + 15)}) != ban_map_t::Type::SOLID) ||
                     (ban_map.get(screen_position_t{s1, (s2 + 15)}) != ban_map_t::Type::SOLID &&
                             ban_map.get(screen_position_t{(s1 + 15), (s2 + 15)}) == ban_map_t::Type::SPRING))) {
                    player.position.y = ((player.position.y >> 16) & 0xfff0) << 16;
                    player.y_add = -400000L;
                    player.anim = 2;
                    player.frame = 0;
                    player.frame_tick = 0;
                    player.image =
                            player_anims[player.anim].frame[player.frame].image + player.direction * 9;
                    player.jump_ready = 0;
                    player.jump_abort = 0;
                    for (auto& object : objects) {
                        if (object.used == 1 && object.type == OBJ_SPRING) {
                            if (ban_map.get(screen_position_t{(s1 + 8), (s2 + 15)}) == ban_map_t::Type::SPRING) {
                                if ((object.position.x >> 20) == ((s1 + 8) >> 4) &&
                                    (object.position.y >> 20) == ((s2 + 15) >> 4)) {
                                    object.frame = 0;
                                    object.ticks = object_anims[object.anim].frame[object.frame].ticks;
                                    object.image = object_anims[object.anim].frame[object.frame].image;
                                    break;
                                }
                            } else {
                                if (ban_map.get(screen_position_t{s1, (s2 + 15)}) == ban_map_t::Type::SPRING) {
                                    if ((object.position.x >> 20) == (s1 >> 4) &&
                                        (object.position.y >> 20) == ((s2 + 15) >> 4)) {
                                        object.frame = 0;
                                        object.ticks = object_anims[object.anim].frame[object.frame].ticks;
                                        object.image = object_anims[object.anim].frame[object.frame].image;
                                        break;
                                    }
                                } else if (ban_map.get(screen_position_t{(s1 + 15), (s2 + 15)}) == ban_map_t::Type::SPRING) {
                                    if ((object.position.x >> 20) == ((s1 + 15) >> 4) &&
                                        (object.position.y >> 20) == ((s2 + 15) >> 4)) {
                                        object.frame = 0;
                                        object.ticks = object_anims[object.anim].frame[object.frame].ticks;
                                        object.image = object_anims[object.anim].frame[object.frame].image;
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    dj_play_sfx(main_info, SFX_SPRING, (unsigned short) (SFX_SPRING_FREQ + rnd(2000) - 1000), 64, 0, -1);
                }
                s1 = (player.position.x >> 16);
                s2 = (player.position.y >> 16);
                if (s2 < 0)
                    s2 = 0;
                if (ban_map.get_by_pixel(s1, s2) == ban_map_t::Type::SOLID || ban_map.get_by_pixel(s1, s2) == ban_map_t::Type::ICE ||
                        ban_map.get_by_pixel(s1, s2) == ban_map_t::Type::SPRING || ban_map.get_by_pixel((s1 + 15), s2) == ban_map_t::Type::SOLID ||
                        ban_map.get_by_pixel((s1 + 15), s2) == ban_map_t::Type::ICE ||
                        ban_map.get_by_pixel((s1 + 15), s2) == ban_map_t::Type::SPRING) {
                    player.y = (((s2 + 16) & 0xfff0)) << 16; //TODO: MASK
                    player.y_add = 0;
                    player.anim = 0;
                    player.frame = 0;
                    player.frame_tick = 0;
                    player.image =
                            player_anims[player.anim].frame[player.frame].image + player.direction * 9;
                }
                s1 = (player.position.x >> 16);
                s2 = (player.position.y >> 16);
                if (s2 < 0)
                    s2 = 0;
                if (ban_map.get_by_pixel((s1 + 8), (s2 + 8)) == ban_map_t::Type::WATER) {
                    if (player.in_water == 0) {
                        /* falling into water */
                        player.in_water = 1;
                        player.anim = 4;
                        player.frame = 0;
                        player.frame_tick = 0;
                        player.image =
                                player_anims[player.anim].frame[player.frame].image + player.direction * 9;
                        if (player.y_add >= 32768) {
                            add_object(OBJ_SPLASH,
                                       screen_position_t{
                                           player.position.to_pixels().x + 8,
                                          (player.position.to_pixels().y & 0xfff0) + 15
                                       }, 0, 0,
                                       OBJ_ANIM_SPLASH, 0);
                            if (blood_is_thicker_than_water == 0)
                                dj_play_sfx(main_info, SFX_SPLASH,
                                            (unsigned short) (SFX_SPLASH_FREQ + rnd(2000) - 1000), 64, 0, -1);
                            else
                                dj_play_sfx(main_info, SFX_SPLASH,
                                            (unsigned short) (SFX_SPLASH_FREQ + rnd(2000) - 5000), 64, 0, -1);
                        }
                    }
                    /* slowly move up to water surface */
                    player.y_add -= 1536;
                    if (player.y_add < 0 && player.anim != 5) {
                        player.anim = 5;
                        player.frame = 0;
                        player.frame_tick = 0;
                        player.image =
                                player_anims[player.anim].frame[player.frame].image + player.direction * 9;
                    }
                    if (player.y_add < -65536L)
                        player.y_add = -65536L;
                    if (player.y_add > 65535L)
                        player.y_add = 65535L;
                    if (ban_map.get_by_pixel(s1, (s2 + 15)) == ban_map_t::Type::SOLID ||
                            ban_map.get_by_pixel(s1, (s2 + 15)) == ban_map_t::Type::ICE ||
                            ban_map.get_by_pixel((s1 + 15), (s2 + 15)) == ban_map_t::Type::SOLID ||
                            ban_map.get_by_pixel((s1 + 15), (s2 + 15)) == ban_map_t::Type::ICE) {
                        player.y = (((s2 + 16) & 0xfff0) - 16) << 16;
                        player.y_add = 0;
                    }
                } else if (ban_map.get_by_pixel(s1, (s2 + 15)) == ban_map_t::Type::SOLID ||
                        ban_map.get_by_pixel(s1, (s2 + 15)) == ban_map_t::Type::ICE ||
                        ban_map.get_by_pixel(s1, (s2 + 15)) == ban_map_t::Type::SPRING ||
                        ban_map.get_by_pixel((s1 + 15), (s2 + 15)) == ban_map_t::Type::SOLID ||
                        ban_map.get_by_pixel((s1 + 15), (s2 + 15)) == ban_map_t::Type::ICE ||
                        ban_map.get_by_pixel((s1 + 15), (s2 + 15)) == ban_map_t::Type::SPRING) {
                    player.in_water = 0;
                    player.y = (((s2 + 16) & 0xfff0) - 16) << 16;
                    player.y_add = 0;
                    if (player.anim != 0 && player.anim != 1) {
                        player.anim = 0;
                        player.frame = 0;
                        player.frame_tick = 0;
                        player.image =
                                player_anims[player.anim].frame[player.frame].image + player.direction * 9;
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
                        player.y = (player.y & 0xffff0000) + 0x10000;
                        player.y_add = 0;
                    }
                    player.in_water = 0;
                }
                if (player.y_add > 36864 && player.anim != 3 && player.in_water == 0) {
                    player.anim = 3;
                    player.frame = 0;
                    player.frame_tick = 0;
                    player.image =
                            player_anims[player.anim].frame[player.frame].image + player.direction * 9;
                }

            }

            player.frame_tick++;
            if (player.frame_tick >= player_anims[player.anim].frame[player.frame].ticks) {
                player.frame++;
                if (player.frame >= player_anims[player.anim].num_frames) {
                    if (player.anim != 6)
                        player.frame = player_anims[player.anim].restart_frame;
                    else
                        position_player(c1);
                }
                player.frame_tick = 0;
            }
            player.image = player_anims[player.anim].frame[player.frame].image + player.direction * 9;

        }

    }

}

void position_player(int player_num) {
    int c1;
    map_position_t position;

    while (true) {

        position = ban_map.get_random_available_floor_position();

        //verifica que el conejo no este cerca de otros conejos
        for (c1 = 0; c1 < players.size(); c1++) {
            if (c1 != player_num && players[c1].enabled == 1) {
                screen_position_t screen_position = position;
                if (abs(screen_position.x - (players[c1].position.to_pixels().x)) < 32 && abs(screen_position.y - (players[c1].position.to_pixels().y)) < 32)
                    break;
            }
        }


        if (c1 == players.size()) {
            players[player_num].set_position(position);

            players[player_num].x_add = players[player_num].y_add = 0;
            players[player_num].direction = 0;
            players[player_num].jump_ready = 1;
            players[player_num].in_water = 0;
            players[player_num].anim = 0;
            players[player_num].frame = 0;
            players[player_num].frame_tick = 0;
            players[player_num].image = player_anims[players[player_num].anim].frame[players[player_num].frame].image;

            players[player_num].dead_flag = 0;

            break;
        }
    }

}

void player_kill(int c1, int c2) {
    auto& player_1 = players[c1];
    auto& player_2 = players[c2];

    if (player_1.y_add >= 0) {

        serverSendKillPacket(c1, c2);
    } else {
        if (player_2.y_add < 0)
            player_2.y_add = 0;
    }
}

void check_collision(int c1, int c2) {
    auto& player_1 = players[c1];
    auto& player_2 = players[c2];
    if (players[c1].enabled == 1 && players[c2].enabled == 1) {
        if (labs(player_1.position.x - players[c2].x) < (12L << 16) &&
            labs(players[c1].y - players[c2].y) < (12L << 16)) {
            if ((labs(players[c1].y - players[c2].y) >> 16) > 5) {
                if (players[c1].y < players[c2].y) {
                    player_kill(c1, c2);
                } else {
                    player_kill(c2, c1);
                }
            } else {
                if (player_1.position.x < players[c2].x) {
                    if (players[c1].x_add > 0)
                        player_1.position.x = players[c2].x - (12L << 16);
                    else if (players[c2].x_add < 0)
                        players[c2].x = player_1.position.x + (12L << 16);
                    else {
                        player_1.position.x -= players[c1].x_add;
                        players[c2].x -= players[c2].x_add;
                    }
                    int l1 = players[c2].x_add;
                    players[c2].x_add = players[c1].x_add;
                    players[c1].x_add = l1;
                    if (players[c1].x_add > 0)
                        players[c1].x_add = -players[c1].x_add;
                    if (players[c2].x_add < 0)
                        players[c2].x_add = -players[c2].x_add;
                } else {
                    if (players[c1].x_add > 0)
                        players[c2].x = player_1.position.x - (12L << 16);
                    else if (players[c2].x_add < 0)
                        player_1.position.x = players[c2].x + (12L << 16);
                    else {
                        player_1.position.x -= players[c1].x_add;
                        players[c2].x -= players[c2].x_add;
                    }
                    int l1 = players[c2].x_add;
                    players[c2].x_add = players[c1].x_add;
                    players[c1].x_add = l1;
                    if (players[c1].x_add < 0)
                        players[c1].x_add = -players[c1].x_add;
                    if (players[c2].x_add > 0)
                        players[c2].x_add = -players[c2].x_add;
                }
            }
        }
    }
}

void collision_check() {
    /* collision check */
    for (int c1 = 0; c1 < players.size(); c1++) {
        for (int c2 = c1 + 1; c2 < players.size() ; c2++) {
            check_collision(c1, c2);

        }
    }
}