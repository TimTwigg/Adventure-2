// updated 28 July 2023

#ifndef LAUNCHER_HPP
#define LAUNCHER_HPP

#include <memory>
#include "Interface.hpp"
#include "GameData.hpp"
#include "GameEngine.hpp"

class Launcher {
    public:
        explicit Launcher(std::shared_ptr<Interface> i);
        Launcher(const Launcher& other) = delete;
        GameEngine* run();
    
    private:
        void menu();
        bool loadGame();
        void newGame();
        void deleteSave();
        void config();
    
        std::shared_ptr<Interface> i;
        GameData data;
};

#endif