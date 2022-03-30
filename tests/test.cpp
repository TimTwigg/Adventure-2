// updated 29 March 2022

#include <gtest/gtest.h>
#include <filesystem>
#include <string>
#include <fstream>

void clear_files() {
	std::string savefile = "saves\\test.game";
    int num{0};
    while (true) {
        std::ifstream i;
        i.open(savefile);
        if (!i) {
            i.close();
            break;
        }
        i.close();
		std::filesystem::remove(savefile);
        ++num;
        savefile = "saves\\test-" + std::to_string(num) + ".game";
    }
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	int out = RUN_ALL_TESTS();
	clear_files();
	return out;
}