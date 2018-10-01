//
// Created by edwin on 01-06-18.
//

#include "player_t.h"
#include "ban_map_t.h"
#include "anim_t.h"
#include "util.h"
#include <iostream>
#include "objects_t.h"
#include "game_manager_t.h"
#include "game_manager_t.h"

namespace {
    void debug_diff(std::string name, int value1, int value2) {
        if ( value1 != value2 ) {
            std::cout << name << value1 << " - " << value2 << " = " << value1 - value2 << "\n";
        }
    }
}

player_t::player_t(game_manager_t& game_manager, int id) : id{id}, game_manager(game_manager) {}

void player_t::set_position(const position_t &position) {
    this->position = position;
}

position_t player_t::get_position() const {
    return this->position;
}

game_manager_t& player_t::get_game_manager() {
    return this->game_manager;
}

void player_t::do_action_left() {

    auto& player = *this;
    auto& ban_map = player.get_game_manager().get_stage().get_map();

    screen_position_t pixel_pos = player.position;
    auto below_left = ban_map.get(pixel_pos + screen_position_t{0, 16});
    auto below = ban_map.get(pixel_pos + screen_position_t{8, 16});
    auto below_right = ban_map.get(pixel_pos + screen_position_t{15, 16});

    if (below == ban_map_t::Type::ICE || ((below_left != ban_map_t::Type::SOLID && below_right == ban_map_t::Type::ICE) ||
               (below_left == ban_map_t::Type::ICE && below_right != ban_map_t::Type::SOLID)) ) {
        if (player.x_add > 0)
            player.x_add -= 1024;
        else
            player.x_add -= 768;
    } else {
        if (player.x_add > 0) {
            player.x_add -= 16384;
            if (player.x_add > -98304L && !player.in_water && below == ban_map_t::Type::SOLID)
                player.get_game_manager().objects.add_smoke(player);
        } else
            player.x_add -= 12288;
    }
    if (player.x_add < -98304L)
        player.x_add = -98304L;
    player.direction = PLAYER_DIRECTION::LEFT;

    if (player.anim_handler.anim == 0) {
        player.set_anim(1);
    }
}

void player_t::do_action_right() {

    auto& player = *this;

    auto& ban_map = player.get_game_manager().get_stage().get_map();

    auto below_left = ban_map.get(player.get_position() + screen_position_t{0, 16});
    auto below = ban_map.get(player.get_position() + screen_position_t{8, 16});
    auto below_right = ban_map.get(player.get_position() + screen_position_t{15, 16});

    if (below == ban_map_t::Type::ICE ||
            ((below_left != ban_map_t::Type::SOLID && below_right == ban_map_t::Type::ICE) ||
            (below_left == ban_map_t::Type::ICE && below_right != ban_map_t::Type::SOLID)) ) {
        if (player.x_add < 0)
            player.x_add += 1024;
        else
            player.x_add += 768;
    } else {
        if (player.x_add < 0) {
            player.x_add += 16384;
            if (player.x_add < 98304L && !player.in_water && below == ban_map_t::Type::SOLID)
                player.get_game_manager().objects.add_smoke(player);
        } else
            player.x_add += 12288;
    }

    if (player.x_add > 98304L)
        player.x_add = 98304L;
    player.direction = PLAYER_DIRECTION::RIGHT;
    if ( player.anim_handler.anim == 0 )
        player.set_anim(1);
}

void player_t::do_no_action() {

    auto& player = *this;

    auto& ban_map = player.get_game_manager().get_stage().get_map();

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
            player.get_game_manager().objects.add_smoke(player);
    }
    if ( player.anim_handler.anim == 1 )
        player.set_anim(0);
}

void player_t::gravity_fall() {

    this->jump_ready = true;
    if (!this->in_water && this->y_add < 0 && this->jump_abort) {
        this->y_add += this->game_manager.get_stage().get_gravity().value;
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

    auto& ban_map = this->game_manager.get_stage().get_map();

    if (ban_map.get(this->get_position() + screen_position_t{8, 15}) == ban_map_t::Type::SPRING ||
        ((ban_map.get(this->get_position() + screen_position_t{0, 15}) == ban_map_t::Type::SPRING &&
          ban_map.get(this->get_position() + screen_position_t{15, 15}) != ban_map_t::Type::SOLID) ||
         (ban_map.get(this->get_position() + screen_position_t{0, 15}) != ban_map_t::Type::SOLID &&
          ban_map.get(this->get_position() + screen_position_t{15, 15}) == ban_map_t::Type::SPRING))) {

        /**
         * this code just animate a spring
         * this can be simplified a lot
         */
        for (auto &object : this->game_manager.objects.objects) {
            screen_position_t screen_position = position;
            if (object.used == 1 && object.type == object_t::Type::SPRING) {
                if (ban_map.get(screen_position + screen_position_t{8, 15}) == ban_map_t::Type::SPRING) {
                    if ((object.position.x.value >> 20) == ((screen_position.x.value + 8) >> 4) &&
                        (object.position.y.value >> 20) == ((screen_position.y.value + 15) >> 4)) {
                        object.set_anim(object.anim_handler.anim, 0);
                        break;
                    }
                } else {
                    if (ban_map.get(screen_position + screen_position_t{0, 15}) == ban_map_t::Type::SPRING) {
                        if ((object.position.x.value >> 20) == (screen_position.x.value >> 4) &&
                            (object.position.y.value >> 20) == ((screen_position.y.value + 15) >> 4)) {
                            object.set_anim(object.anim_handler.anim, 0);
                            break;
                        }
                    } else if (ban_map.get(screen_position + screen_position_t{15, 15}) == ban_map_t::Type::SPRING) {
                        if ((object.position.x.value >> 20) == ((screen_position.x.value + 15) >> 4) &&
                            (object.position.y.value >> 20) == ((screen_position.y.value + 15) >> 4)) {
                            object.set_anim(object.anim_handler.anim, 0);
                            break;
                        }
                    }
                }
            }
        }


        this->position.y = ((this->position.y.value >> 16) & 0xfff0) << 16;
        this->y_add = -400000L;
        this->set_anim(2);
        this->jump_ready = false;
        this->jump_abort = false;
        this->game_manager.sound_manager.play_sfx_spring();

    }
}

void player_t::check_ceiling() {

    auto& ban_map = this->game_manager.get_stage().get_map();

    auto player_bounding_box = this->get_bounding_box_for_walls();

    auto ceilings = ban_map.get(player_bounding_box.get_top_box());
    //if the bunny collide in the top with a solid wall


    if ( ceilings.is_ceil() ) {
        auto lower_ceil = ceilings.get_lowest_ceil();

        auto top = lower_ceil.bounding_box.get_bottom();
        top.value += 1;

        this->position.y = top;
        this->y_add = 0;

        this->set_anim(0);
    }
}


void player_t::check_lateral_walls() {

    //left boundary of the map
    if ((this->position.x.value >> 16) < 0) {
        this->position.x = 0;
        this->x_add = 0;
    }

    //right boundary of the map
    if ((this->position.x.value >> 16) + 15 > 351) {
        this->position.x = 336L << 16;
        this->x_add = 0;
    }

    auto& ban_map = this->get_game_manager().get_stage().get_map();

    auto player_bounding_box = this->get_bounding_box_for_walls();

    auto colliding_blocks = ban_map.get(player_bounding_box);

    auto left_walls = colliding_blocks.collide(player_bounding_box.get_left_box());

    // if the bunny collide in the left with a wall |B
      if ( left_walls.is_wall() ) {
        auto left_wall =left_walls.get_rightmost_wall();

        auto right = left_wall.bounding_box.get_right();
        right.value += 1;

        this->position.x = right;
        this->x_add = 0;
    }

    auto right_walls = colliding_blocks.collide(player_bounding_box.get_right_box());

    // if the bunny collide in the right with a wall    B|
        if (right_walls.is_wall()) {

        auto right_wall = right_walls.get_leftmost_wall();

        auto left = right_wall.bounding_box.get_left();
        left.value -= player_bounding_box.width;

        this->position.x = left;

        this->x_add = 0;
    }
}

void player_t::update_movement() {
    this->action_left = this->control.left_pressed();
    this->action_right = this->control.right_pressed();
    this->action_up = this->control.up_pressed();
}

void player_t::position_player() {

    auto& ban_map = this->get_game_manager().get_stage().get_map();

    map_position_t position;


    again:

        position = ban_map.get_random_available_floor_position();

        //verifica que el conejo no este cerca de otros conejos
        for (const auto &other_player : this->get_game_manager().players) {
            if (other_player.get_id() != this->get_id()) {
                screen_position_t screen_position = position;
                if (abs(screen_position.x.value - (screen_position_t{other_player.get_position()}.x.value)) < 32 &&
                    abs(screen_position.y.value - (screen_position_t{other_player.get_position()}.y.value)) < 32)
                    goto again;
            }
        }

    this->set_position(position);

    this->x_add = 0;
    this->y_add = 0;
    this->direction = PLAYER_DIRECTION::RIGHT;
    this->jump_ready = true;
    this->in_water = false;
    this->anim_handler.set_anim(0);
    this->dead_flag = false;

}

void player_t::player_kill(player_t &player_1, player_t &player_2) {

    auto& game_manager =player_1.get_game_manager();

    if (player_2.y_add < 0)
        player_2.y_add = 0;

    if ( !player_1.is_moving_up()) {
        game_manager.kill(player_1.get_id(), player_2.get_id());
     }
}

void player_t::check_collision(player_t &player_1, player_t &player_2) {

    if ( player_1.collide(player_2) ) {
        if (player_1.is_over(player_2) ) {
            player_kill(player_1, player_2);

        } else if (player_2.is_over(player_1) ) {
            player_kill(player_2, player_1);

        } else {
            auto fix_player = [](player_t &player_1, player_t &player_2) {
                if (player_1.x_add > 0)
                    player_1.position.x = player_2.position.x.value - (12L << 16);
                else if (player_2.x_add < 0)
                    player_2.position.x = player_1.position.x + (12L << 16);
                else {
                    player_1.position.x -= player_1.x_add;
                    player_2.position.x -= player_2.x_add;
                }

                std::swap(player_1.x_add, player_2.x_add);

                if (player_1.x_add > 0)
                    player_1.x_add = -player_1.x_add;
                if (player_2.x_add < 0)
                    player_2.x_add = -player_2.x_add;
            };

            if (player_1.position.x < player_2.position.x) {
                fix_player(player_1, player_2);
            } else {
                fix_player(player_2, player_1);
            }
        }
    }
}

void player_t::reset_kills() {
    this->bumps = 0;
    this->bumped = std::vector<int>(this->game_manager.players.size(), 0);
}

void player_t::set_anim(int anim) {
    this->anim_handler.set_anim(anim);
}

void player_t::animate() {

    auto& player_anims = this->get_game_manager().player_anims;

    this->anim_handler.animate(player_anims,this->direction == player_t::PLAYER_DIRECTION::LEFT ? 9 : 0);
}

void player_t::do_action_up() {
    auto& player = *this;

    auto& game_manager = player.get_game_manager();

    auto& ban_map = game_manager.get_stage().get_map();

    if (!player.jetpack) {
        /* no jetpack */
        if (player.pogostick || (player.jump_ready && player.action_up)) {

            auto below_boxes = ban_map.get(player.get_bounding_box_for_walls().get_below_box());
            below_boxes = below_boxes.just_below(player.get_bounding_box_for_walls().get_bottom());

            /* jump */
            if ( below_boxes.is_floor() ) {
                player.y_add = -280000L;
                player.set_anim(2);
                player.jump_ready = false;
                player.jump_abort = true;
                if (!player.pogostick) {
                    game_manager.sound_manager.play_sfx_jump();
                } else {
                    game_manager.sound_manager.play_sfx_spring();
                }
            }
            /* jump out of water */
            if (ban_map.is_in_water(player.get_position())) {
                player.y_add = -196608L;
                player.in_water = false;
                player.set_anim(2);
                player.jump_ready = false;
                player.jump_abort = true;
                if (!player.pogostick) {
                    game_manager.sound_manager.play_sfx_jump();
                } else {
                    game_manager.sound_manager.play_sfx_spring();
                }

            }
        }
        /* fall down by gravity */
        if (!player.pogostick && (!player.action_up)) {
            player.gravity_fall();
        }
    } else {
        /* with jetpack */
        if (player.action_up) {
            player.y_add = std::max(player.y_add - 16384, -400000);

            if (ban_map.is_in_water(player.get_position()))
                player.in_water = false;
            if (rnd(100) < 50)
                game_manager.objects.add_jetpack_smoke(player);
        }
    }
}

bool player_t::check_floor() {
    auto& player = *this;
    auto& ban_map = this->get_game_manager().get_stage().get_map();

    auto player_bounding_box = player.get_bounding_box_for_walls();

    auto floors = ban_map.get(player_bounding_box.get_bottom_box());

    if ( !floors.is_floor() ) return false;

    auto top = floors.get_highest_floor().bounding_box.get_top();
    top.value -= player_bounding_box.height;

    player.position.y = top;

    player.y_add = 0;

    return true;

}

void player_t::do_falling() {

    auto& player = *this;
    auto& ban_map = this->get_game_manager().get_stage().get_map();

    screen_position_t screen_position = player.get_position();




    auto player_bounding_box = player.get_bounding_box_for_walls();

    auto floors = ban_map.get(player_bounding_box.get_bottom_box());

    //is in water
    if (ban_map.get(screen_position + screen_position_t{8, 8}) == ban_map_t::Type::WATER) {
        //enter in water
        if (!player.in_water) {
            /* falling into water */
            player.in_water = true;
            player.set_anim(4);


            if (player.y_add >= 32768) {
                screen_position_t screen_position = player.get_position();
                screen_position.y.value &= 0xfff0;
                game_manager.fx_splash(screen_position
                                         + screen_position_t{9, 15});
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

        //check floor in water

        player.check_floor();
        //check is you are in the floor

    } else if (player.check_floor() ) {

        player.in_water = false;

        if (player.anim_handler.anim != 0 && player.anim_handler.anim != 1) {
            player.set_anim(0);
        }
    } else {
        if (!player.in_water) {
            //water floatability, double in space
            player.y_add = std::min(player.y_add + 12288, 327680);


        } else {
            player.position.y = (player.position.y.value & 0xffff0000) + 0x10000;
            player.y_add = 0;
        }
        player.in_water = false;
    }

    if (player.y_add > 36864 && player.anim_handler.anim != 3 && !player.in_water) {
        player.set_anim(3);
    }
}