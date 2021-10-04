#pragma once

#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader();
    ~Shader();

    void CompileFromSource(const char* vertexSource, const char* fragmentSource);
    void Compile(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void Link();
    void AddAttribute(const std::string& attributeName);
    GLint GetUniformLocation(const std::string& uniformName);
    void Use();
    void Unuse();
    void Dispose();

private:
    void CompileShader(const char* source, const std::string& name, GLuint id);

    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    int numAttributes;
};
