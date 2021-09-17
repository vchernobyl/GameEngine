#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

Texture TextureCache::GetTexture(const std::string& texturePath) {
    const auto iter = textureMap.find(texturePath);

    if (iter == textureMap.end()) {
	Texture newTexture = ImageLoader::LoadPNG(texturePath);
	textureMap.emplace(texturePath, newTexture);
	return newTexture;
    }

    return iter->second;
}
