#include "Shader.h"
#include "Errors.h"
#include <fstream>
#include <vector>

Shader::Shader() : programID(0), vertexShaderID(0), fragmentShaderID(0), numAttributes(0) {
}

Shader::~Shader() {
}

void Shader::Compile(const std::string& vertexShaderFilePath, const std::string& fragmentShaderFilePath) {
    programID = glCreateProgram();

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    if (vertexShaderID == 0) {
	FatalError("Failed to create a vertex shader.");
    }

    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragmentShaderID == 0) {
	FatalError("Failed to create a fragment shader.");
    }

    CompileShader(vertexShaderFilePath, vertexShaderID);
    CompileShader(fragmentShaderFilePath, fragmentShaderID);
}

void Shader::Link() {
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);

    GLint isLinked = 0;
    glGetProgramiv(programID, GL_LINK_STATUS, &isLinked);
    if (isLinked == GL_FALSE) {
	GLint maxLength = 0;
	glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &maxLength);

	std::vector<char> errorLog(maxLength);
	glGetProgramInfoLog(programID, maxLength, &maxLength, &errorLog[0]);

	glDeleteProgram(programID);
	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	std::printf("%s\n", &(errorLog[0]));
	FatalError("Shaders failed to link.");
    }

    glDetachShader(programID, vertexShaderID);
    glDetachShader(programID, fragmentShaderID);
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

void Shader::AddAttribute(const std::string& attributeName) {
    glBindAttribLocation(programID, numAttributes++, attributeName.c_str());
}

GLint Shader::GetUniformLocation(const std::string& uniformName) {
    GLuint location = glGetUniformLocation(programID, uniformName.c_str());
    if (location == GL_INVALID_INDEX) {
	FatalError("Uniform " + uniformName + " not found in shader.");
    }
    return location;
}

void Shader::Use() {
    glUseProgram(programID);
    for (int i = 0; i < numAttributes; i++) {
	glEnableVertexAttribArray(i);
    }
}

void Shader::Unuse() {
    glUseProgram(0);
    for (int i = 0; i < numAttributes; i++) {
	glDisableVertexAttribArray(i);
    }
}

void Shader::CompileShader(const std::string& filePath, GLuint id) {
    std::ifstream vertexFile(filePath);
    if (vertexFile.fail()) {
	FatalError("Failed to open shader file: " + filePath);
    }

    std::string fileContents = "";
    std::string line;

    while (std::getline(vertexFile, line)) {
	fileContents += line + "\n";
    }

    vertexFile.close();

    const char* contentsPtr = fileContents.c_str();
    glShaderSource(id, 1, &contentsPtr, nullptr);
    glCompileShader(id);

    GLint success = 0;
    glGetShaderiv(id, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE) {
	GLint maxLength = 0;
	glGetShaderiv(id, GL_INFO_LOG_LENGTH, &maxLength);

	std::vector<char> errorLog(maxLength);
	glGetShaderInfoLog(id, maxLength, &maxLength, &errorLog[0]);
	glDeleteShader(id);

	std::printf("%s\n", &(errorLog[0]));
	FatalError("Shader " + filePath + " failed to compile.");
    }
}
