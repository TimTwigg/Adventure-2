// Updated 5 September 2023

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <memory>
#include "Storage.hpp"
#include "Resource.hpp"
#include "AdventureException.hpp"

TEST(storageTests, constructor) {
    Storage s{"backpack"};
}

TEST(storageTests, fromString) {
    Storage* s = Storage::fromString("STORAGE, backpack");
    delete s;

    Storage* s2 = Storage::fromString("STORAGE, backpack; RESOURCE, stone, 3; CRESOURCE, arrow, 4");
    delete s2;
}

TEST(storageTests, store) {
    Storage s{"backpack"};
    ASSERT_EQ(s.store(std::shared_ptr<Object>(new Resource("stone", 2))), nullptr);
    ASSERT_EQ(s.getWeight(), 25);
    
    std::shared_ptr<Object> obj = std::shared_ptr<Object>(new Resource("stone", 2));
    s.store(obj);
    ASSERT_EQ(s.getWeight(), 35);
    Resource* r = static_cast<Resource*>(obj.get());
    ASSERT_EQ(r->getCount(), 1);
}

TEST(storageTests, remove) {
    Storage s{"backpack"};
    s.store(std::shared_ptr<Object>(new Resource("stone")));
    s.store(std::shared_ptr<Object>(new Resource("wood", 2)));

    ASSERT_EQ(s.getWeight(), 27);
    std::shared_ptr<Object> o = s.remove("stone");
    ASSERT_EQ(o->getName(), "stone");
    ASSERT_EQ(o->getType(), OBJCLASS::RESOURCE);
    ASSERT_EQ(s.getWeight(), 17);

    o = s.remove("wood", 2);
    ASSERT_EQ(o->getName(), "wood");
    ASSERT_EQ(o->getType(), OBJCLASS::RESOURCE);
    Resource* r = static_cast<Resource*>(o.get());
    ASSERT_EQ(r->getCount(), 2);
    ASSERT_EQ(s.getWeight(), 5);

    ASSERT_THROW(s.remove("whatever"), AdventureException);
}