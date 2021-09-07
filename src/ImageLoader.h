#pragma once

#include "Texture.h"
#include <string>

class ImageLoader {
public:
    static Texture LoadPNG(const std::string& filePath);
};
