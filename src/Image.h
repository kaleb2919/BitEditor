#pragma once

#include <iostream>
#include <glad/glad.h>

class Image
{
    GLuint descriptor;
    GLuint texture_wrap = GL_CLAMP_TO_EDGE;
    GLenum data_format = GL_RGB;
    GLenum internal_format = GL_RGB8;

    int width, height, channels;
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
    unsigned char* getData();
    static void save(char const* file_name, GLuint width, GLuint height, int comp, const unsigned char* buffer);
};
