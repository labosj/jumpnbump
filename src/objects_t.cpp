//
// Created by edwin on 11-06-18.
//

#include "objects_t.h"
#include "globals.h"
#include "gob_t.h"
#include "util.h"
#include "game_manager_t.h"

objects_t objects;

void objects_t::add(game_manager_t& game_manager, object_t::Type type, const position_t &position, int x_add, int y_add, int anim, int frame) {

    for (auto &object : this->objects) {
        if (object.used == 0) {
            object = object_t{game_manager, type, position, x_add, y_add, anim, frame};
            return;
        }
    }

    this->objects.emplace_back(game_manager, type, position, x_add, y_add, anim, frame);

}

void objects_t::update(game_manager_t& game_manager) {
    for (auto& object : this->objects) {

        if (object.is_used()) {
            switch (object.type) {
                case object_t::Type::SPRING:
                    object.update_spring();
                    if (object.is_used() )
                        game_manager.pobs.add(object.get_position(), object.anim_handler.image, &game_manager.object_gobs);
                    break;
                case object_t::Type::SPLASH:
                    object.update_splash();
                    if (object.is_used() )
                        game_manager.pobs.add(object.get_position(), object.anim_handler.image, &game_manager.object_gobs);
                    break;
                case object_t::Type::SMOKE:
                    object.update_smoke();
                    if (object.is_used() )
                        game_manager.pobs.add(object.get_position(), object.anim_handler.image, &game_manager.object_gobs);
                    break;
                case object_t::Type::YEL_BUTFLY:
                case object_t::Type::PINK_BUTFLY:
                    object.update_butterfly();
                    if (object.is_used() )
                        game_manager.pobs.add(object.get_position(), object.anim_handler.image, &game_manager.object_gobs);
                    break;
                case object_t::Type::FUR:
                    object.update_fur();
                    if (object.is_used() ) {
                        int s1 = (int) (atan2(object.y_add, object.x_add) * 4 / M_PI);
                        if (s1 < 0)
                            s1 += 8;
                        if (s1 < 0)
                            s1 = 0;
                        if (s1 > 7)
                            s1 = 7;
                        game_manager.pobs.add(object.get_position(), object.anim_handler.frame + s1, &game_manager.object_gobs);
                    }
                    break;
                case object_t::Type::FLESH:
                    object.update_flesh();
                    if (object.is_used())
                        game_manager.pobs.add(object.get_position(), object.anim_handler.frame, &game_manager.object_gobs);
                    break;
                case object_t::Type::FLESH_TRACE:
                    object.update_flesh_trace();

                    if (object.is_used() )
                        game_manager.pobs.add(object.get_position(), object.anim_handler.image, &game_manager.object_gobs);
                    break;
            }
        }
    }
}

void objects_t::add_smoke(player_t &player) {
    this->add(player.get_game_manager(), object_t::Type::SMOKE,
                player.get_position() +
                screen_position_t{2 + rnd(9), 13 + rnd(5)}, 0,
                -16384 - rnd(8192), OBJ_ANIM_SMOKE, 0);
}

void objects_t::add_jetpack_smoke(player_t &player) {
    this->add(player.get_game_manager(), object_t::Type::SMOKE, player.get_position() + screen_position_t{6 + rnd(5), 10 + rnd(5)},
                0, 16384 + rnd(8192), OBJ_ANIM_SMOKE, 0);
}

