// Updated: 12 June 2022

#include <memory>
#include "WindowsInterface.hpp"
#include "Launcher.hpp"
#include "GameEngine.hpp"

int main() {
    std::shared_ptr<Interface> i{new WindowsInterface()};
    Launcher launcher{i};
    std::unique_ptr<GameEngine> engine{launcher.run()};
    engine->run();
    return 0;
}