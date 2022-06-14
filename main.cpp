// Updated: 14 June 2022

#include <memory>
#include "WindowsInterface.hpp"
#include "Launcher.hpp"
#include "GameEngine.hpp"
#include "AdventureException.hpp"

int main() {
    std::shared_ptr<Interface> i{new WindowsInterface()};
    Launcher launcher{i};
    try {
        std::unique_ptr<GameEngine> engine{launcher.run()};
        engine->run();
    }
    catch (AdventureException e) {
        i->output(e.getReason());
    }
    return 0;
}