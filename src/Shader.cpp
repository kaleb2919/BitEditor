#include "Shader.h"

#include <fstream>
#include <iostream>
#include <string>
#include <glad/glad.h>

bool Shader::read(const char* file_path, std::string& content)
{
    std::ifstream file_stream(file_path, std::ios::in);

    if(!file_stream.is_open()) {
        std::cout << "Could not read file " << file_path << ". File does not exist." << std::endl;
        return false;
    }

    std::string line = "";
    while(!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return true;
}


Shader::Shader()
{
    program_descriptor = glCreateProgram();
}

bool Shader::loadShaders(const char* vertex_shader_file_name, const char* fragment_shader_file_name)
{
    GLuint vertex_descriptor = GL_NONE;
    if (!loadShader(vertex_shader_file_name, GL_VERTEX_SHADER, vertex_descriptor))
    {
        return false;
    }

    GLuint fragment_descriptor = GL_NONE;
    if (!loadShader(fragment_shader_file_name, GL_FRAGMENT_SHADER, fragment_descriptor))
    {
        return false;
    }

    glLinkProgram(program_descriptor);

    int  success;
    glGetProgramiv(program_descriptor, GL_LINK_STATUS, &success);
    if(!success) {
        checkCompileErrors(program_descriptor, GL_PROGRAM);
        return false;
    }

    glDeleteShader(fragment_descriptor);
    glDeleteShader(vertex_descriptor);

    return true;
}

bool Shader::loadSrcShaders(const GLchar* vertex_shader, const GLchar* fragment_shader)
{
    GLuint vertex_descriptor = GL_NONE;
    if (!loadSrcShader(vertex_shader, GL_VERTEX_SHADER, vertex_descriptor))
    {
        return false;
    }

    GLuint fragment_descriptor = GL_NONE;
    if (!loadSrcShader(fragment_shader, GL_FRAGMENT_SHADER, fragment_descriptor))
    {
        return false;
    }

    glLinkProgram(program_descriptor);

    int  success;
    glGetProgramiv(program_descriptor, GL_LINK_STATUS, &success);
    if(!success) {
        checkCompileErrors(program_descriptor, GL_PROGRAM);
        return false;
    }

    glDeleteShader(fragment_descriptor);
    glDeleteShader(vertex_descriptor);

    return true;
}

void Shader::use()
{
    glUseProgram(program_descriptor);
}

bool Shader::loadShader(const char* file_name, GLuint type_shader, GLuint& shader_descriptor)
{
    shader_descriptor = glCreateShader(type_shader);
    std::string source;
    read(file_name, source);
    const GLchar* source_ptr = source.c_str();
    glShaderSource(shader_descriptor, 1, &source_ptr, nullptr);
    glCompileShader(shader_descriptor);

    int  success;
    glGetShaderiv(shader_descriptor, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        checkCompileErrors(shader_descriptor, GL_SHADER);
        return false;
    }

    glAttachShader(program_descriptor, shader_descriptor);

    return true;
}


bool Shader::loadSrcShader(const GLchar* shader, GLuint type_shader, GLuint& shader_descriptor)
{
    shader_descriptor = glCreateShader(type_shader);
    glShaderSource(shader_descriptor, 1, &shader, nullptr);
    glCompileShader(shader_descriptor);

    int  success;
    glGetShaderiv(shader_descriptor, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        checkCompileErrors(shader_descriptor, GL_SHADER);
        return false;
    }

    glAttachShader(program_descriptor, shader_descriptor);

    return true;
}

void Shader::checkCompileErrors(GLuint shader, GLuint type)
{
    GLint success;
    GLchar info_log[1024];
    if (type != GL_PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR" << std::endl << info_log << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, info_log);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR" << std::endl << info_log << std::endl;
        }
    }
}

GLuint Shader::getProgramDescriptor()
{
    return program_descriptor;
}

void Shader::setBool(const char* name, bool value) const
{
    glUniform1i(glGetUniformLocation(program_descriptor, name), static_cast<int>(value));
}

void Shader::setInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(program_descriptor, name), value);
}

void Shader::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(program_descriptor, name), value);
}

void Shader::setVec2(const char* name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(program_descriptor, name), 1, &value[0]);
}

void Shader::setVec2(const char* name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(program_descriptor, name), x, y);
}

void Shader::setVec3(const char* name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(program_descriptor, name), 1, &value[0]);
}

void Shader::setVec3(const char* name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(program_descriptor, name), x, y, z);
}

void Shader::setVec4(const char* name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(program_descriptor, name), 1, &value[0]);
}

void Shader::setVec4(const char* name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(program_descriptor, name), x, y, z, w);
}

void Shader::setMat2(const char* name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(program_descriptor, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const char* name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(program_descriptor, name), 1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const char* name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_descriptor, name), 1, GL_FALSE, &mat[0][0]);
}
