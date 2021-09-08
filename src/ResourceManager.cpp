#include "ResourceManager.h"

TextureCache ResourceManager::textureCache;;

Texture ResourceManager::GetTexture(const std::string& texturePath) {
    return textureCache.GetTexture(texturePath);
}
