#include "Errors.h"
#include <iostream>
#include <SDL/SDL.h>

void FatalError(const std::string& error) {
    std::cout << error << std::endl;
    std::cout << "Enter any key to quit...";
    int temp;
    std::cin >> temp;
    SDL_Quit();
    exit(1);
}

