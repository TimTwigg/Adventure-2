// Updated 21 September 2023

#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include "Object.hpp"

class Storage : public Object {
    public:
        static Storage* fromString(const std::string& code);

    public:
        explicit Storage(std::string name);
        operator std::string() const noexcept override;
        std::string printString() const noexcept override;
        bool isCarryable() const noexcept;
        std::map<std::string, unsigned int> getRecipe() const noexcept;
        // store items in the storage unit
        // returns any items which did not fit, returning nullptr if everything was added
        std::shared_ptr<Object> store(std::shared_ptr<Object> obj);
        std::shared_ptr<Object> remove(std::string item, int count = 1);
        int getWeight() const noexcept override;
        int getCapacity() const noexcept;
        bool contains(std::string item);

    private:
        std::vector<std::string> stringifyContents() const noexcept;

        bool carry;
        int capacity;
        std::map<std::string, unsigned int> recipe;
        std::vector<std::shared_ptr<Object>> contents;
};

#endif