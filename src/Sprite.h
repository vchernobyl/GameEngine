#pragma once

#include <GL/glew.h>
#include <string>
#include "Texture.h"


class Sprite {
public:
    Sprite();
    ~Sprite();

    void Init(float x, float y, float width, float height, const std::string& texturePath);
    void Draw();

private:
    float x;
    float y;
    float width;
    float height;

    GLuint vboID;
    Texture texture;
};
