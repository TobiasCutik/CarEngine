#pragma once
#include <filesystem>
#include <map>

namespace CE
{
    class SaveGameManager {

    public:
        SaveGameManager() = default;

        static void writeToSaveGame(const std::filesystem::path& file, const std::map<std::string, std::string>& stringMap);
        static std::map<std::string, std::string> readFromSaveGame(const std::filesystem::path& file);

    private:
        static void writeStringToSaveGame(const std::string& string, std::ofstream& outStream);
        static std::string readStringFromSaveGame(std::ifstream& inStream);

        static void writeMapSizeToSaveGame(uint16_t mapSize, std::ofstream& outStream);
        static uint16_t readMapSizeFromSaveGame(std::ifstream& inStream);

        static bool isStringWithinSizeLimit(const std::string& string);
    };
}
