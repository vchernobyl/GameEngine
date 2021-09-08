#pragma once

#include "TextureCache.h"

class ResourceManager {
public:
    static Texture GetTexture(const std::string& texturePath);
private:
    static TextureCache textureCache;
};
