// Updated: 8 December 2021

#include <map>
#include <vector>
#include <string>

namespace JS {
    void dump(const std::map<std::string, std::string>& m, const std::string& filename);
    void dump(const std::vector<std::string> v, const std::string& filename);
}