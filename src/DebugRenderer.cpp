#include "DebugRenderer.h"

const float PI = 3.14159f;

const char* VertexSource = R"(
#version 130
in vec2 vertexPosition;
in vec4 vertexColor;

out vec2 fragmentPosition;
out vec4 fragmentColor;

uniform mat4 P;

void main() {
    gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
    gl_Position.z = 0.0;
    gl_Position.w = 1.0;

    fragmentPosition = vertexPosition;
    fragmentColor = vertexColor;
}
)";

const char* FragmentSource = R"(
#version 130
in vec2 fragmentPosition;
in vec4 fragmentColor;

out vec4 color;

void main() {
    color = fragmentColor;
}
)";

glm::vec2 RotatePoint(const glm::vec2& point, float angle) {
    glm::vec2 rotated;
    rotated.x = point.x * cos(angle) - point.y * sin(angle);
    rotated.y = point.x * sin(angle) + point.y * cos(angle);
    return rotated;
}

DebugRenderer::DebugRenderer() {
    shader.CompileFromSource(VertexSource, FragmentSource);
    shader.AddAttribute("vertexPosition");
    shader.AddAttribute("vertexColor");
    shader.Link();

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ibo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, position));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(DebugVertex), (void*)offsetof(DebugVertex, color));

    glBindVertexArray(0);
}

DebugRenderer::~DebugRenderer() {
    Dispose();
}

void DebugRenderer::End() {
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(DebugVertex), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(DebugVertex), vertices.data());
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), nullptr, GL_DYNAMIC_DRAW);
    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, indices.size() * sizeof(GLuint), indices.data());
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    elementCount = indices.size();
    vertices.clear();
    indices.clear();
}

void DebugRenderer::DrawBox(const glm::vec4& rect, const ColorRGBA8& color, float angle) {
    glm::vec2 halfDims(rect.z / 2.0f, rect.w / 2.0f);

    glm::vec2 tl(-halfDims.x, halfDims.y);
    glm::vec2 bl(-halfDims.x, -halfDims.y);
    glm::vec2 br(halfDims.x, -halfDims.y);
    glm::vec2 tr(halfDims.x, halfDims.y);

    size_t i = vertices.size();
    vertices.resize(vertices.size() + 4);

    glm::vec2 positionOffset(rect.x, rect.y);

    vertices[i + 0].position = RotatePoint(tl, angle) + halfDims + positionOffset;
    vertices[i + 1].position = RotatePoint(bl, angle) + halfDims + positionOffset;
    vertices[i + 2].position = RotatePoint(br, angle) + halfDims + positionOffset;
    vertices[i + 3].position = RotatePoint(tr, angle) + halfDims + positionOffset;

    vertices[i + 0].color = color;
    vertices[i + 1].color = color;
    vertices[i + 2].color = color;
    vertices[i + 3].color = color;

    indices.reserve(indices.size() + 8);

    indices.push_back(i);
    indices.push_back(i + 1);

    indices.push_back(i + 1);
    indices.push_back(i + 2);
    
    indices.push_back(i + 2);
    indices.push_back(i + 3);

    indices.push_back(i + 3);
    indices.push_back(i);
}

void DebugRenderer::DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius) {
    static const int vertexCount = 100;
    size_t start = vertices.size();

    vertices.resize(start + vertexCount);

    for (int i = 0; i < vertexCount; i++) {
	float angle = static_cast<float>(i) / vertexCount * 2 * PI;
	vertices[start + i].position.x = cos(angle) * radius + center.x;
	vertices[start + i].position.y = sin(angle) * radius + center.y;
	vertices[start + i].color = color;
    }

    indices.reserve(indices.size() + vertexCount * 2);

    for (int i = 0; i < vertexCount - 1; i++) {
	indices.push_back(start + i);
	indices.push_back(start + i + 1);
    }

    indices.push_back(start + vertexCount - 1);
    indices.push_back(start);
}

void DebugRenderer::Render(const glm::mat4& projectionMatrix, float lineWidth) {
    shader.Use();

    GLint pUniform = shader.GetUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    glBindVertexArray(vao);
    glLineWidth(lineWidth);
    glDrawElements(GL_LINES, elementCount, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    shader.Unuse();
}

void DebugRenderer::Dispose() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ibo) glDeleteBuffers(1, &ibo);

    shader.Dispose();
}
