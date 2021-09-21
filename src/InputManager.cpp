#include "InputManager.h"

void InputManager::Update() {
    for (auto& entry : keyMap) {
	previousKeyMap[entry.first] = entry.second;
    }
}

void InputManager::PressKey(unsigned int keyID) {
    keyMap[keyID] = true;
}

void InputManager::ReleaseKey(unsigned int keyID) {
    keyMap[keyID] = false;
}

bool InputManager::IsKeyDown(unsigned int keyID) const {
    const auto it = keyMap.find(keyID);
    if (it != keyMap.end()) {
	return it->second;
    }
    return false;
}

bool InputManager::IsKeyPressed(unsigned int keyID) const {
    if (IsKeyDown(keyID) && !WasKeyDown(keyID)) {
	return true;
    }
    return false;
}

void InputManager::SetMouseCoords(float x, float y) {
    mouseCoords.x = x;
    mouseCoords.y = y;
}

bool InputManager::WasKeyDown(unsigned int keyID) const {
    const auto it = previousKeyMap.find(keyID);
    if (it != previousKeyMap.end()) {
	return it->second;
    }
    return false;
}
