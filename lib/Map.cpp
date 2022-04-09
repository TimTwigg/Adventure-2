// updated 9 April 2022

#include <map>
#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include "Map.hpp"
#include "RandomGenerator.hpp"
#include "FileReader.hpp"
#include "json.hpp"
using json = nlohmann::json;

Location::Location(RandomGenerator& gen) {
    json data = FileReader::readFile("biomes.json");
    std::vector<std::string> biomes;
    for (auto [k, _] : data.items()) biomes.push_back(k);
    int index = gen.getRandInt(0, biomes.size()-1);
    biome = biomes[index];
    data = data[biome];

    if (biome == "Forest" || biome == "Jungle" || (data["trees"].get<bool>() && gen.getRandBool())) {
        here.push_back("trees");
        if (biome == "Forest" || biome == "Jungle" || gen.getRandBool()) {
            int count = gen.getRandInt(0, 3);
            for (int i = 0; i < count; ++i) here.push_back("wood");
        }
    }

    if (biome == "Mountain" || (data["stone"].get<bool>() && gen.getRandBool())) {
        here.push_back("stone deposit");
        if (biome == "Mountain" || gen.getRandBool()) {
            int count = gen.getRandInt(0, 3);
            for (int i = 0; i < count; ++i) here.push_back("stone");
        }
    }

    if (data["metal"].get<bool>() && gen.getRandBool()) here.push_back("metal deposit");
    if (data["gold"].get<bool>() && gen.getRandBool()) here.push_back("gold deposit");
    if (biome != "River" && gen.getRandBool() && gen.getRandInt(0, 10) * data["river"].get<double>() >= gen.getRandInt(1, 10)) here.push_back("river");
    std::vector<std::string> civs = data["civ-types"].get<std::vector<std::string>>();
    if (civs.size() > 0 && gen.getRandBool() && gen.getRandBool()) here.push_back(civs[gen.getRandInt(0, civs.size()-1)]);
    std::vector<std::string> animals = data["animals"].get<std::vector<std::string>>();
    if (animals.size() > 0) {
        for (auto& a : animals) {
            if (gen.getRandBool()) here.push_back(a);
        }
    }
    // TODO: enemies
}

Location::Location(const std::string& biome, const std::vector<std::string>& here) : biome{biome}, here{here} {
}