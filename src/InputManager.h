#pragma once

#include <unordered_map>
#include <glm/glm.hpp>

class InputManager {
public:
    void Update();

    void PressKey(unsigned int keyID);
    void ReleaseKey(unsigned int keyID);

    bool IsKeyDown(unsigned int keyID) const;
    bool IsKeyPressed(unsigned int keyID) const;

    void SetMouseCoords(float x, float y);
    glm::vec2 GetMouseCoords() const { return mouseCoords; }
    
private:
    bool WasKeyDown(unsigned int keyID) const;
    
    std::unordered_map<unsigned int, bool> keyMap;
    std::unordered_map<unsigned int, bool> previousKeyMap;
    glm::vec2 mouseCoords;
};
