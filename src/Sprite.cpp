#include "Sprite.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include <cstddef>

Sprite::Sprite() {
    vboID = 0;
}

Sprite::~Sprite() {
    if (vboID != 0) {
	glDeleteBuffers(1, &vboID);
    }
}

void Sprite::Init(float x, float y, float width, float height, const std::string& texturePath) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    texture = ResourceManager::GetTexture(texturePath);

    if (vboID == 0) {
	glGenBuffers(1, &vboID);
    }

    Vertex vertexData[6];

    // First triangle.
    vertexData[0].SetPosition(x + width, y + height);
    vertexData[0].SetUV(1.0f, 1.0f);

    vertexData[1].SetPosition(x, y + height);
    vertexData[1].SetUV(0.0f, 1.0f);

    vertexData[2].SetPosition(x, y);
    vertexData[2].SetUV(0.0f, 0.0f);

    // Second triangle.
    vertexData[3].SetPosition(x, y);
    vertexData[3].SetUV(0.0f, 0.0f);
    
    vertexData[4].SetPosition(x + width, y);
    vertexData[4].SetUV(1.0f, 0.0f);

    vertexData[5].SetPosition(x + width, y + height);
    vertexData[5].SetUV(1.0f, 1.0f);

    for (int i = 0; i < 6; i++) {
	vertexData[i].SetColor(255.0f, 0.0f, 255.0f, 255.0f);
    }

    vertexData[1].SetColor(0.0f, 0.0f, 255.0f, 255.0f);

    vertexData[4].SetColor(255.0f, 255.0f, 0.0f, 255.0f);


    glBindBuffer(GL_ARRAY_BUFFER, vboID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::Draw() {
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindBuffer(GL_ARRAY_BUFFER, vboID);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);    
}
