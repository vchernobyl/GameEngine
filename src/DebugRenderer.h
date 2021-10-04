#pragma once

#include "Vertex.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <GL/glew.h>
#include <vector>

class DebugRenderer {
public:
    DebugRenderer();
    ~DebugRenderer();
    void End();
    void DrawBox(const glm::vec4& rect, const ColorRGBA8& color, float angle);
    void DrawCircle(const glm::vec2& center, const ColorRGBA8& color, float radius);
    void Render();
    void Dispose();

private:

    struct DebugVertex {
	glm::vec2 position;
	ColorRGBA8 color;
    };
    
    Shader shader;
    GLuint vao, vbo, ibo;

    std::vector<DebugVertex> vertices;
    std::vector<GLuint> indices;
};
