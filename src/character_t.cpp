//
// Created by edwin on 20-09-18.
//

#include <nlohmann/json.hpp>
#include <fstream>
#include "character_t.h"

void character_t::load(const std::string& folder) {
    this->texture.loadFromFile(folder + "/sprites.png");
    this->gobs.add_character(folder + "/data.json", this->texture);


    std::ifstream file{folder + "/data.json"};
    nlohmann::json json;
    file >> json;


    auto read_anim = [&](const nlohmann::json& element) {
        anim_t anim;
        anim.restart_frame = element["restart_frame"];
        for ( auto& frame : element["frames"] ) {
            anim.frame.emplace_back(frame["sprite"], frame["duration"]);
        }
        return anim;
    };

    this->anims.emplace_back(read_anim(json["anim_data"]["standing"]));
    this->anims.emplace_back(read_anim(json["anim_data"]["running"]));
    this->anims.emplace_back(read_anim(json["anim_data"]["jump_up"]));
    this->anims.emplace_back(read_anim(json["anim_data"]["falling"]));
    this->anims.emplace_back(read_anim(json["anim_data"]["dive"]));
    this->anims.emplace_back(read_anim(json["anim_data"]["float_up"]));
    this->anims.emplace_back(read_anim(json["anim_data"]["stomped"]));
}