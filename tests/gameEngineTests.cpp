// updated 11 June 2022

#include <gtest/gtest.h>
#include <memory>
#include "GameEngine.hpp"
#include "AdventureException.hpp"
#include "WindowsInterface.hpp"
#include "SkillSets.hpp"

namespace {
    std::shared_ptr<Interface> i{new WindowsInterface()};
    GameData data{SkillSets::TRAVELER, "test"};
}

TEST(gameEngineTests, constructor) {
    GameEngine engine{i, data};
}

TEST(gameEngineTests, save) {
    GameEngine engine{i, data};
    engine.save();
}