#pragma once

#include "Vertex.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

enum class SpriteSortType {
    None,
    FrontToBack,
    BackToFront,
    Texture
};

struct SpriteBatchItem {
    GLuint texture;
    float depth;
    Vertex topLeft;
    Vertex bottomLeft;
    Vertex bottomRight;
    Vertex topRight;
};

class RenderBatch {
public:
    RenderBatch(GLuint offset, GLuint numVertices, GLuint texture) :
	offset(offset), numVertices(numVertices), texture(texture) {}
    
    GLuint offset;
    GLuint numVertices;
    GLuint texture;
};

class SpriteBatch {
public:
    SpriteBatch();
    void Init();
    void Begin(SpriteSortType sortType = SpriteSortType::Texture);
    void End();
    void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
	      GLuint texture, float depth, const Color& color);
    void DrawBatch();

private:
    void CreateRenderBatches();
    void CreateVertexArray();
    void SortSpriteBatchItems();

    GLuint vbo;
    GLuint vao;
    SpriteSortType sortType;
    std::vector<SpriteBatchItem*> spriteBatchItems;
    std::vector<RenderBatch> renderBatches;
};
