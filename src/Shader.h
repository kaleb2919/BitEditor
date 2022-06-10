#pragma once
#include <xstring>
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
    GLuint program_descriptor = GL_NONE;

    bool read(const char* file_path, std::string& content);
    bool loadShader(const char * file_name, GLuint type_shader, GLuint& shader_descriptor);
    bool loadSrcShader(const GLchar* shader, GLuint type_shader, GLuint& shader_descriptor);
    void checkCompileErrors(GLuint shader, GLuint type);
public:
    Shader();

    bool loadShaders(const char * vertex_shader_file_name, const char * fragment_shader_file_name);
    bool loadSrcShaders(const GLchar* vertex_shader, const GLchar* fragment_shader);

    void use();

    GLuint getProgramDescriptor();

    void setBool(const char* name, bool value) const;
    void setInt(const char* name, int value) const;
    void setFloat(const char* name, float value) const;
    void setVec2(const char* name, const glm::vec2& value) const;
    void setVec2(const char* name, float x, float y) const;
    void setVec3(const char* name, const glm::vec3& value) const;
    void setVec3(const char* name, float x, float y, float z) const;
    void setVec4(const char* name, const glm::vec4& value) const;
    void setVec4(const char* name, float x, float y, float z, float w) const;
    void setMat2(const char* name, const glm::mat2& mat) const;
    void setMat3(const char* name, const glm::mat3& mat) const;
    void setMat4(const char* name, const glm::mat4& mat) const;
};
