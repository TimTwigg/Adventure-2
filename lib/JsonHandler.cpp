// Updated: 8 December 2021

#include <iostream>
#include <fstream>
#include "JsonHandler.hpp"
#include "..\\lib2\\json.hpp"

using json = nlohmann::json;

void JS::dump(const std::map<std::string, std::string>& m, const std::string& filename) {

}

void JS::dump(const std::vector<std::string> v, const std::string& filename) {
    for (auto i : v) {
        std::cout << i << std::endl;
    }
}