// Updated: 9 December 2021

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include "SkillSets.hpp"
#include "..\\tp_lib\\json.hpp"
using json = nlohmann::json;

class Player {
    public:
        Player(SkillSets skillset, std::string savepath);
        Player(const Player& other) = delete;
        void save() const;
        void level_up();
        json getSet(SkillSets skillset);

        json data;
    
    private:
        void level_stats();
};

#endif