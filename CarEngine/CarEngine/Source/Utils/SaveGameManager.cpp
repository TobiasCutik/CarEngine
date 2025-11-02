#include "SaveGameManager.h"
#include <fstream>
#include <iostream>
#include <string>

namespace CE
{
    void SaveGameManager::writeToSaveGame(const std::filesystem::path& file, const std::map<std::string, std::string>& stringMap)
    {
        std::ofstream outStream(file, std::ios::binary | std::ios::out);
        if (!outStream.is_open())
        {
            std::cout << "Failed to open " << file << std::endl;
        }
        else
        {
            writeMapSizeToSaveGame(stringMap.size(), outStream);

            for (const auto& [key, value] : stringMap)
            {
                if (!isStringWithinSizeLimit(key))
                    continue;
                if (!isStringWithinSizeLimit(value))
                    continue;

                writeStringToSaveGame(key, outStream);
                writeStringToSaveGame(value, outStream);
            }

            outStream.close();
        }
    }

    std::map<std::string, std::string> SaveGameManager::readFromSaveGame(const std::filesystem::path& file)
    {
        std::map<std::string, std::string> stringMap;

        std::ifstream inStream(file, std::fstream::binary | std::fstream::in);
        if (!inStream.is_open())
        {
            std::cout << "Failed to open " << file << std::endl;
        }
        else
        {
            int mapSize = readMapSizeFromSaveGame(inStream);

            for (int i = 0; i < mapSize; ++i)
                stringMap.insert({readStringFromSaveGame(inStream), readStringFromSaveGame(inStream)});

            inStream.close();
        }

        return stringMap;
    }

    void SaveGameManager::writeStringToSaveGame(const std::string& string, std::ofstream& outStream)
    {
        uint16_t stringLength = string.length();
        uint8_t stringLengthBytes[2];
        stringLengthBytes[0] = (stringLength) & 0xFF;
        stringLengthBytes[1] = (stringLength >> 8) & 0xFF;

        outStream.write(reinterpret_cast<char*>(stringLengthBytes), 2);
        outStream.write(string.c_str(), stringLength);
    }

    std::string SaveGameManager::readStringFromSaveGame(std::ifstream& inStream)
    {
        uint16_t stringLength;
        uint8_t stringLengthBytes[2];

        inStream.read(reinterpret_cast<char*>(stringLengthBytes), 2);
        stringLength = stringLengthBytes[0] | (stringLengthBytes[1] << 8);

        char* stringBuffer = new char[stringLength];
        inStream.read(stringBuffer, stringLength);
        std::string string = std::string(stringBuffer, stringLength);

        delete[] stringBuffer;

        return string;
    }

    void SaveGameManager::writeMapSizeToSaveGame(uint16_t mapSize, std::ofstream& outStream)
    {
        uint8_t mapSizeBytes[2];
        mapSizeBytes[0] = (mapSize) & 0xFF;
        mapSizeBytes[1] = (mapSize >> 8) & 0xFF;

        outStream.write(reinterpret_cast<char*>(mapSizeBytes), 2);
    }

    uint16_t SaveGameManager::readMapSizeFromSaveGame(std::ifstream& inStream)
    {
        uint8_t mapSizeBytes[2];
        inStream.read(reinterpret_cast<char*>(mapSizeBytes), 2);
        return mapSizeBytes[0] | (mapSizeBytes[1] << 8);
    }

    bool SaveGameManager::isStringWithinSizeLimit(const std::string& string)
    {
        auto sizeLimit = std::numeric_limits<uint16_t>::max();
        if (string.length() > sizeLimit)
        {
            std::cout << "String \"" << string << "\" is too large. Only a length of " << sizeLimit << " is allowed."<< std::endl;
            return false;
        }

        return true;
    }
}