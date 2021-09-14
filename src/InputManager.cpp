#include "InputManager.h"

void InputManager::PressKey(unsigned int keyID) {
    keyMap[keyID] = true;
}

void InputManager::ReleaseKey(unsigned int keyID) {
    keyMap[keyID] = false;
}

void InputManager::SetMouseCoords(float x, float y) {
    mouseCoords.x = x;
    mouseCoords.y = y;
}

bool InputManager::IsKeyPressed(unsigned int keyID) {
    const auto it = keyMap.find(keyID);
    if (it != keyMap.end()) {
	return it->second;
    }
    return false;
}
