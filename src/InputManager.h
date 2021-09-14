#pragma once

#include <unordered_map>

class InputManager {
public:
    void PressKey(unsigned int keyID);
    void ReleaseKey(unsigned int keyID);
    bool IsKeyPressed(unsigned int keyID);
private:
    std::unordered_map<unsigned int, bool> keyMap;
};
