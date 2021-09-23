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
		    GLuint texture, float depth, const ColorRGBA8& color) :
	texture(texture), depth(depth) {

	topLeft.color = color;
	topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
	topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

	bottomLeft.color = color;
	bottomLeft.SetPosition(destRect.x, destRect.y);
	bottomLeft.SetUV(uvRect.x, uvRect.y);

	bottomRight.color = color;
	bottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
	bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

	topRight.color = color;
	topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
	topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);
    }
    
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
	      GLuint texture, float depth, const ColorRGBA8& color);
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
