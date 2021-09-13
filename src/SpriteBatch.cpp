#include "SpriteBatch.h"
#include <iostream>
#include <algorithm>

SpriteBatch::SpriteBatch() : vbo(0), vao(0) {
}

void SpriteBatch::Init() {
    CreateVertexArray();
}

void SpriteBatch::Begin(SpriteSortType sortType) {
    this->sortType = sortType;
    spriteBatchItems.clear();

    for (SpriteBatchItem* batchItem : spriteBatchItems) {
	delete batchItem;
    }

    renderBatches.clear();
}

void SpriteBatch::End() {
    SortSpriteBatchItems();
    CreateRenderBatches();
}

void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
		       GLuint texture, float depth, const Color& color) {
    SpriteBatchItem* batchItem = new SpriteBatchItem;
    batchItem->texture = texture;
    batchItem->depth = depth;

    batchItem->topLeft.color = color;
    batchItem->topLeft.SetPosition(destRect.x, destRect.y + destRect.w);
    batchItem->topLeft.SetUV(uvRect.x, uvRect.y + uvRect.w);

    batchItem->bottomLeft.color = color;
    batchItem->bottomLeft.SetPosition(destRect.x, destRect.y);
    batchItem->bottomLeft.SetUV(uvRect.x, uvRect.y);

    batchItem->bottomRight.color = color;
    batchItem->bottomRight.SetPosition(destRect.x + destRect.z, destRect.y);
    batchItem->bottomRight.SetUV(uvRect.x + uvRect.z, uvRect.y);

    batchItem->topRight.color = color;
    batchItem->topRight.SetPosition(destRect.x + destRect.z, destRect.y + destRect.w);
    batchItem->topRight.SetUV(uvRect.x + uvRect.z, uvRect.y + uvRect.w);

    spriteBatchItems.push_back(batchItem);
}

void SpriteBatch::DrawBatch() {
    glBindVertexArray(vao);

    for (int i = 0; i < renderBatches.size(); i++) {
	glBindTexture(GL_TEXTURE_2D, renderBatches[i].texture);
	glDrawArrays(GL_TRIANGLES, renderBatches[i].offset, renderBatches[i].numVertices);
    }

    glBindVertexArray(0);
}

void SpriteBatch::CreateRenderBatches() {
    if (spriteBatchItems.empty()) {
	return;
    }

    std::vector<Vertex> vertices;
    vertices.resize(spriteBatchItems.size() * 6);

    renderBatches.emplace_back(0, 6, spriteBatchItems[0]->texture);

    int currentVertex = 0;
    vertices[currentVertex++] = spriteBatchItems[0]->topLeft;
    vertices[currentVertex++] = spriteBatchItems[0]->bottomLeft;
    vertices[currentVertex++] = spriteBatchItems[0]->bottomRight;

    vertices[currentVertex++] = spriteBatchItems[0]->bottomRight;
    vertices[currentVertex++] = spriteBatchItems[0]->topRight;
    vertices[currentVertex++] = spriteBatchItems[0]->topLeft;

    for (int i = 1; i < spriteBatchItems.size(); i++) {
	if (spriteBatchItems[i]->texture != spriteBatchItems[i - 1]->texture) {
	    renderBatches.emplace_back(currentVertex, 6, spriteBatchItems[i]->texture);
	} else {
	    renderBatches.back().numVertices += 6;
	}
	
	vertices[currentVertex++] = spriteBatchItems[i]->topLeft;
	vertices[currentVertex++] = spriteBatchItems[i]->bottomLeft;
	vertices[currentVertex++] = spriteBatchItems[i]->bottomRight;

	vertices[currentVertex++] = spriteBatchItems[i]->bottomRight;
	vertices[currentVertex++] = spriteBatchItems[i]->topRight;
	vertices[currentVertex++] = spriteBatchItems[i]->topLeft;
    }

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(Vertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::CreateVertexArray() {
    if (vao == 0) glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    if (vbo == 0) glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
    
    glBindVertexArray(0);
}

void SpriteBatch::SortSpriteBatchItems() {
    switch (sortType) {
    case SpriteSortType::BackToFront:
	std::stable_sort(spriteBatchItems.begin(), spriteBatchItems.end(),
			 [](SpriteBatchItem* a, SpriteBatchItem* b) { return a->depth > b->depth; });
	break;
    case SpriteSortType::FrontToBack:
	std::stable_sort(spriteBatchItems.begin(), spriteBatchItems.end(),
			 [](SpriteBatchItem* a, SpriteBatchItem* b) { return a->depth < b->depth; });
	break;
    case SpriteSortType::Texture:
	std::stable_sort(spriteBatchItems.begin(), spriteBatchItems.end(),
			 [](SpriteBatchItem* a, SpriteBatchItem* b) { return a->texture < b->texture; });
	break;
    }
}
