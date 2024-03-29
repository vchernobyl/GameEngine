#include "ImageLoader.h"
#include "IOManager.h"
#include "Errors.h"
#include "picopng.h"
#include <vector>

Texture ImageLoader::LoadPNG(const std::string& filePath) {
    std::vector<unsigned char> in;
    if (IOManager::ReadFileToBuffer(filePath, in) == false) {
	FatalError("Failed to load PNG file to buffer.");
    }
    
    std::vector<unsigned char> out;
    unsigned long width, height;
    int errorCode = decodePNG(out, width, height, &(in[0]), in.size());
    if (errorCode != 0) {
	FatalError("Failed to decode PNG: " + std::to_string(errorCode));
    }

    Texture texture;
    texture.width = width;
    texture.height = height;

    glGenTextures(1, &(texture.id));
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &(out[0]));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);

    return texture;
}
