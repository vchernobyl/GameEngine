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

class SpriteBatchItem {
public:
    SpriteBatchItem(const glm::vec4& destRect, const glm::vec4& uvRect,
		    GLuint texture, float depth, const ColorRGBA8& color);

    SpriteBatchItem(const glm::vec4& destRect, const glm::vec4& uvRect,
		    GLuint texture, float depth, const ColorRGBA8& color, float angle);
    
    GLuint texture;
    float depth;
    Vertex topLeft;
    Vertex bottomLeft;
    Vertex bottomRight;
    Vertex topRight;

private:
    glm::vec2 RotatePoint(const glm::vec2& point, float angle);
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
	      GLuint texture, float depth, const ColorRGBA8& color);

    void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
	      GLuint texture, float depth, const ColorRGBA8& color, float angle);

    void Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
	      GLuint texture, float depth, const ColorRGBA8& color, const glm::vec2& direction);

    void DrawBatch();

private:
    void CreateRenderBatches();
    void CreateVertexArray();
    void SortSpriteBatchItems();

    GLuint vbo;
    GLuint vao;
    SpriteSortType sortType;

    std::vector<SpriteBatchItem*> spriteBatchItemPtrs;
    std::vector<SpriteBatchItem> spriteBatchItems;
    std::vector<RenderBatch> renderBatches;
};
