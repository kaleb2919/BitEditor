#include "Image.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define __STDC_LIB_EXT1__
#include <stb_image_write.h>

int Image::getPixelSize()
{
    switch (internal_format)
    {
    case GL_RGBA8:
        return 4;
    case GL_RGB8:
        return 3;
    default: ;
        return 3;
    }
}

int Image::getWidth()
{
    return width;
}

int Image::getHeight()
{
    return height;
}

Image::Image(const char* file_name)
{
    stbi_set_flip_vertically_on_load(true);

    data = stbi_load(file_name, &width, &height, &channels, 0);

    if (data)
    {
        if (channels == 4)
        {
            internal_format = GL_RGBA8;
            data_format = GL_RGBA;
        }
        else if (channels == 3)
        {
            internal_format = GL_RGB8;
            data_format = GL_RGB;
        }
        else
        {
            std::cout << "Texture Format Not Supported, Channels: " << channels << std::endl;
        }

        std::cout << "Loaded image. Width: " << width << " Height: " << height << " Channels: " << channels << std::endl;
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

}

Image::Image(GLint width, GLint height, GLint channels) :
    width(width), height(height), channels(channels)
{
    int rgb_size = height * (width * 3);
    data = new unsigned char[rgb_size];

    for (int i = 0; i < rgb_size; i = i + 3)
    {
        data[i] = 0;
        data[i+1] = 0;
        data[i+2] = 0;
    }
}

GLuint Image::getDescriptor()
{
    return descriptor;
}

void Image::reload()
{
    glDeleteTextures(1, &descriptor);
    init();
}

void Image::save(char const* file_name)
{
    stbi_write_png(file_name, width, height, getPixelSize(), data, 0);
}

unsigned char* Image::getData()
{
    return data;
}

void Image::save(char const* file_name, GLuint width, GLuint height, int comp, const unsigned char* buffer)
{
    stbi_write_png(file_name, width, height, comp, buffer, 0);
}

void Image::init()
{
    glCreateTextures(GL_TEXTURE_2D, 1, &descriptor);
    glTextureStorage2D(descriptor, 1, internal_format, width, height);
    glTextureSubImage2D(descriptor, 0, 0, 0, width, height, data_format, GL_UNSIGNED_BYTE, data);
    glGenerateTextureMipmap(GL_TEXTURE_2D);
    glTextureParameteri(descriptor, GL_TEXTURE_WRAP_S, texture_wrap);
    glTextureParameteri(descriptor, GL_TEXTURE_WRAP_T, texture_wrap);
    glTextureParameteri(descriptor, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(descriptor, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}
