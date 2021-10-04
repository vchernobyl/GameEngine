#include "DebugRenderer.h"

DebugRenderer::DebugRenderer() {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glBindVertexArray(0);
}

DebugRenderer::~DebugRenderer() {
    Dispose();
}

void DebugRenderer::End() {}

void DebugRenderer::DrawBox(const glm::vec4& rect, const ColorRGBA8& color, float angle) {}

void DebugRenderer::DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius) {}

void DebugRenderer::Render() {}

void DebugRenderer::Dispose() {}
