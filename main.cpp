// Updated: 7 July 2022

#include <memory>
#include <fstream>
#include <ctime>
#include <string>
#include <sstream>
#include <filesystem>
#include "WindowsInterface.hpp"
#include "Launcher.hpp"
#include "GameEngine.hpp"
#include "AdventureException.hpp"
namespace fs = std::filesystem;

void logError(const std::string& error) {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream ss;
    ss << std::put_time(&tm, "%d_%m_%Y");
    std::string filename = "err_" + ss.str() + ".txt";
    ss.str(std::string());
    ss << std::put_time(&tm, "%H:%M:%S");

    fs::path p{"logs"};
    if (!fs::exists(p)) fs::create_directory(p);
    std::ofstream f;
    f.open("logs\\" + filename, std::ios_base::app);
    f << "[" << ss.str() << "] " << error << std::endl;
    f.close();
}

int main() {
    std::shared_ptr<Interface> i{new WindowsInterface()};
    Launcher launcher{i};
    try {
        std::unique_ptr<GameEngine> engine{launcher.run()};
        engine->run();
    }
    catch (AdventureException e) {
        i->output(e.getReason(), Color::RED_LIGHT);
        i->output("Check error log for details", Color::RED_LIGHT);
        logError("ADVENTURE_EXCEPTION: " + e.getReason());
    }
    catch (const std::exception& e) {
        i->output("UNKNOWN ERROR", Color::RED_DARK);
        i->output("Check error log for details", Color::RED_LIGHT);
        logError("UNKNOWN_ERROR: " + static_cast<std::string>(e.what()));
    }
    return 0;
}