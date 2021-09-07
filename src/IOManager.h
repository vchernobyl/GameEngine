#pragma once

#include <string>
#include <vector>

class IOManager {
public:
    static bool ReadFileToBuffer(const std::string& filePath, std::vector<unsigned char>& buffer);
};
