#pragma once

using namespace geode::prelude;

class Globals {

    private:

        Globals() {
            isShipCube = Mod::get()->getSavedValue<bool>("isShipCube", true);
            isBallCube = Mod::get()->getSavedValue<bool>("isBallCube", true);
            isUFOCube = Mod::get()->getSavedValue<bool>("isUFOCube", true);
            isWaveCube = Mod::get()->getSavedValue<bool>("isWaveCube", true);
            isRobotCube = Mod::get()->getSavedValue<bool>("isRobotCube", true);
            isSpiderCube = Mod::get()->getSavedValue<bool>("isSpiderCube", true);
            isSwingCube = Mod::get()->getSavedValue<bool>("isSwingCube", true);
        };

    public:

        Globals(const Globals&) = delete;
        Globals& operator=(const Globals&) = delete;

        bool isShipCube = true;
        bool isBallCube = true;
        bool isUFOCube = true;
        bool isWaveCube = true;
        bool isRobotCube = true;
        bool isSpiderCube = true;
        bool isSwingCube = true;

        static Globals& get() {
            static Globals instance;
            return instance;
        }

};