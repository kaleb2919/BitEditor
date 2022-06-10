#pragma once

#include <glad/glad.h>
#include <glm/vec3.hpp>

class Image
{
    GLuint descriptor = GL_NONE;
    GLint texture_wrap = GL_CLAMP_TO_EDGE;
    GLenum data_format = GL_RGB;
    GLenum internal_format = GL_RGB8;

    int width = 0;
    int height = 0;
    int channels = 0;

    unsigned char* data;

protected:
    int getPixelSize();

public:
    Image(const char* file_name);
    Image(GLint width, GLint height, GLint channels);
    GLuint getDescriptor();
    int getWidth();
    int getHeight();
    void init();
    void reload();
    void save(char const* file_name);
    void setColor(int x, int y, glm::u8vec3 color);
    unsigned char* copyData();
    static void save(char const* file_name, GLint width, GLint height, int comp, const unsigned char* buffer);
};
