#pragma once

#include <string>

enum WindowFlags {
    Invisible  = 0x1,
    Fullscreen = 0x2,
    Borderless = 0x4
};

class Window {
public:
    int Create(const std::string& windowName, int screenWidth, int screenHeight, unsigned int flags);
    void SwapBuffer();
    int GetScreenWidth() const { return screenWidth; }
    int GetScreenHeight() const { return screenHeight; }
private:
    class SDL_Window* sdlWindow;
    int screenWidth, screenHeight;
};
