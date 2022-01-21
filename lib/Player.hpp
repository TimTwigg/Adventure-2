// Updated: 20 January 2022

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <time.h>
#include "SkillSets.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Player {
    public:
        static json getSet(SkillSets skillset, unsigned int seed = time(NULL));

    public:
        Player(SkillSets skillset, std::string savepath);
        Player(const Player& other) = delete;
        void save() const;
        void level_up();

        json data;
    
    private:
        void level_stats();
};

#endif