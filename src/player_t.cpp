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
    if (this->in_water == 0 && this->y_add < 0 && this->jump_abort) {
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

    //if the bunny collide in the top with a solid wall
    if (ban_map.is_solid(this->get_position() + screen_position_t{0,  0}) ||
        ban_map.is_solid(this->get_position() + screen_position_t{15, 0}) ) {
        //stop the velocity in y
        this->position.y = (((screen_position_t{this->get_position()}.y.value + 16) & 0xfff0)) << 16; //TODO: MASK
        this->y_add = 0;
        this->set_anim(0);
    }
}


void player_t::check_lateral_walls() {

    auto& ban_map = this->get_game_manager().get_stage().get_map();

    // if the bunny collide in the left with a wall |B
    if (ban_map.is_solid(this->get_position() + screen_position_t{0,  0}) ||
        ban_map.is_solid(this->get_position() + screen_position_t{0, 15})) {
        int s1 = (this->position.x.value >> 16);
        this->position.x = (((s1 + 16) & 0xfff0)) << 16;
        this->x_add = 0;
    }


    // if the bunny collide in the right with a wall    B|
    if (ban_map.is_solid(this->get_position() + screen_position_t{15, 0}) ||
        ban_map.is_solid(this->get_position() + screen_position_t{15, 15})) {
        int s1 = (this->position.x.value >> 16);
        this->position.x = (((s1 + 16) & 0xfff0) - 16) << 16;
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
    this->in_water = 0;
    this->anim_handler.anim = 0;
    this->anim_handler.frame = 0;
    this->anim_handler.frame_tick = 0;
    this->anim_handler.image = this->game_manager.player_anims[this->anim_handler.anim].frame[this->anim_handler.frame].image;

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