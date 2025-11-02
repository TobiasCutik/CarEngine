#include "SaveGame.h"
#include <utility>

namespace CE
{
    SaveGame::SaveGame(std::filesystem::path file, std::string name) : file(std::move(file)), name(std::move(name))
    {
    }

    void SaveGame::load()
    {
        data = CE::SaveGameManager::readFromSaveGame(file);
    }

    void SaveGame::save() const
    {
        CE::SaveGameManager::writeToSaveGame(file, data);
    }
}