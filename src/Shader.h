#pragma once

#include <string>
#include <GL/glew.h>

class Shader {
public:
    Shader();
    ~Shader();
    
    void Compile(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath);
    void Link();
    void AddAttribute(const std::string& attributeName);
    GLint GetUniformLocation(const std::string& uniformName);
    void Use();
    void Unuse();

private:
    void CompileShader(const std::string& filePath, GLuint id);
    
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    int numAttributes;
};
