// updated 30 April 2022

#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <cstdlib>
namespace fs = std::filesystem;

void clear_files() {
	std::string savefile = "saves\\test";
    int num{0};
    while (true) {
        if (!fs::exists(fs::path(savefile))) break;
        std::string s = "rmdir " + savefile + " /S /Q";
		int i = system(s.c_str());
        ++num;
        savefile = "saves\\test-" + std::to_string(num);
    }
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	int out = RUN_ALL_TESTS();
	clear_files();
	return out;
}