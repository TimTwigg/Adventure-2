// updated 27 May 2024

#include <gtest/gtest.h>
#include "Enemy.hpp"
#include "AdventureException.hpp"

TEST(enemyTests, constructor) {
    Enemy a{"bandit"};
    Enemy b{" BandIT "};
    ASSERT_THROW(Enemy c{"orcs"}, AdventureException);
    ASSERT_THROW(Enemy c{""}, AdventureException);
}

TEST(enemyTests, attack) {
    Enemy a{"bandit"};
    double d = a.attack();
    ASSERT_NE(d, 0);
}

TEST(enemyTests, attackDMG) {
    Enemy a{"bandit"};
    float hp = a.getHP();
    bool b = a.attack(10);
    ASSERT_FALSE(b);
    ASSERT_EQ(a.getHP(), hp-10);

    b = a.attack(90);
    ASSERT_FALSE(b);
    ASSERT_EQ(a.getHP(), hp-100);

    Enemy a2 = Enemy("bandit");
    b = a2.attack(hp);
    ASSERT_TRUE(b);
    ASSERT_EQ(a2.getHP(), 0);
}

TEST(enemyTests, getDrops) {
    Enemy a{"bandit"};
    const std::vector<std::shared_ptr<Object>>& drops = a.getDrops();
}

TEST(enemyTests, fromString) {
    Enemy a{"bandit"};
    std::string s = a.operator std::string();
    Enemy* b = Enemy::fromString(s);
    ASSERT_EQ(a.getName(), b->getName());
    ASSERT_EQ(a.getHP(), b->getHP());
    delete b;

    s = "ENEMY, bandit, 100";
    b = Enemy::fromString(s);
    ASSERT_EQ(b->getName(), "bandit");
    ASSERT_EQ(b->getHP(), 100);
    delete b;
}