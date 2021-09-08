#include "TextureCache.h"
#include "ImageLoader.h"
#include <iostream>

Texture TextureCache::GetTexture(const std::string& texturePath) {
    const auto iter = textureMap.find(texturePath);

    if (iter == textureMap.end()) {
	Texture newTexture = ImageLoader::LoadPNG(texturePath);
	textureMap.emplace(texturePath, newTexture);

	std::cout << "Saving texture to cache.\n";
	return newTexture;
    }

    std::cout << "Loaded texture from cache.\n";
    return iter->second;
}
