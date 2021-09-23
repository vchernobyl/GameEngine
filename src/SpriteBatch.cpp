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
    renderBatches.clear();
}

void SpriteBatch::End() {
    spriteBatchItemPtrs.resize(spriteBatchItems.size());
    for (int i = 0; i < spriteBatchItems.size(); i++) {
	spriteBatchItemPtrs[i] = &spriteBatchItems[i];
    }
    
    SortSpriteBatchItems();
    CreateRenderBatches();
}

void SpriteBatch::Draw(const glm::vec4& destRect, const glm::vec4& uvRect,
		       GLuint texture, float depth, const ColorRGBA8& color) {
    spriteBatchItems.emplace_back(destRect, uvRect, texture, depth, color);
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
    if (spriteBatchItemPtrs.empty()) {
	return;
    }

    std::vector<Vertex> vertices;
    vertices.resize(spriteBatchItemPtrs.size() * 6);

    renderBatches.emplace_back(0, 6, spriteBatchItemPtrs[0]->texture);

    int currentVertex = 0;
    vertices[currentVertex++] = spriteBatchItemPtrs[0]->topLeft;
    vertices[currentVertex++] = spriteBatchItemPtrs[0]->bottomLeft;
    vertices[currentVertex++] = spriteBatchItemPtrs[0]->bottomRight;

    vertices[currentVertex++] = spriteBatchItemPtrs[0]->bottomRight;
    vertices[currentVertex++] = spriteBatchItemPtrs[0]->topRight;
    vertices[currentVertex++] = spriteBatchItemPtrs[0]->topLeft;

    for (int i = 1; i < spriteBatchItemPtrs.size(); i++) {
	if (spriteBatchItemPtrs[i]->texture != spriteBatchItemPtrs[i - 1]->texture) {
	    renderBatches.emplace_back(currentVertex, 6, spriteBatchItemPtrs[i]->texture);
	} else {
	    renderBatches.back().numVertices += 6;
	}
	
	vertices[currentVertex++] = spriteBatchItemPtrs[i]->topLeft;
	vertices[currentVertex++] = spriteBatchItemPtrs[i]->bottomLeft;
	vertices[currentVertex++] = spriteBatchItemPtrs[i]->bottomRight;

	vertices[currentVertex++] = spriteBatchItemPtrs[i]->bottomRight;
	vertices[currentVertex++] = spriteBatchItemPtrs[i]->topRight;
	vertices[currentVertex++] = spriteBatchItemPtrs[i]->topLeft;
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
	std::stable_sort(spriteBatchItemPtrs.begin(), spriteBatchItemPtrs.end(),
			 [](SpriteBatchItem* a, SpriteBatchItem* b) { return a->depth > b->depth; });
	break;
    case SpriteSortType::FrontToBack:
	std::stable_sort(spriteBatchItemPtrs.begin(), spriteBatchItemPtrs.end(),
			 [](SpriteBatchItem* a, SpriteBatchItem* b) { return a->depth < b->depth; });
	break;
    case SpriteSortType::Texture:
	std::stable_sort(spriteBatchItemPtrs.begin(), spriteBatchItemPtrs.end(),
			 [](SpriteBatchItem* a, SpriteBatchItem* b) { return a->texture < b->texture; });
	break;
    }
}
