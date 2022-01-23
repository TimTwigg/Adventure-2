// Updated: 22 January 2022

#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <vector>
#include "SkillSets.hpp"
#include "json.hpp"
using json = nlohmann::json;

class Player {
    public:
        Player(SkillSets skillset, std::string savepath);
        explicit Player(std::string savepath);
        Player(const Player& other) = delete;
        void save() const;
        void level_up();
        double stat(const std::string& stat_name) const;
        std::vector<std::string> getStatNames() const noexcept;
    
    private:
        json data;
        std::vector<std::string> invalid_stat_names;

        void level_stats();
};

#endif