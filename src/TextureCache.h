#pragma once

#include <map>
#include <string>
#include "Texture.h"

class TextureCache {
public:
    Texture GetTexture(const std::string& texturePath);
private:
    std::map<std::string, Texture> textureMap;
};
