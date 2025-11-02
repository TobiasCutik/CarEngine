#pragma once
#include "SaveGameManager.h"

namespace CE
{
    class SaveGame {

    public:
        explicit SaveGame(std::filesystem::path file, std::string name = "New SaveGame");

        void load();
        void save() const;

        std::map<std::string, std::string> data;
        std::string name;

    private:
        std::filesystem::path file;
    };
}