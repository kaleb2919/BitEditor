#include "Field.h"
#include <utility>

Field::Field(unsigned char * data, int width, int height)
    : width(width), height(height)
{
    buffer = data;
}

void Field::getColorCell(int x, int y, unsigned char bit_mask, unsigned char color_mask, glm::u8vec3 &color)
{
    int position = (x + width * y) * 3;

    if (color_mask & R)
    {
        color.r = buffer[position] & bit_mask;
    }
    else
    {
        color.r = 0;
    }

    if (color_mask & G)
    {
        color.g = buffer[position + 1] & bit_mask;
    }
    else
    {
        color.g = 0;
    }

    if (color_mask & B)
    {
        color.b = buffer[position + 2] & bit_mask;
    }
    else
    {
        color.b = 0;
    }
}

int Field::getWidth()
{
    return width;
}

int Field::getHeight()
{
    return height;
}

unsigned char* Field::copyBuffer(unsigned char bit_mask, unsigned char color_mask)
{
    int rgb_size = height * (width * 3);
    auto new_buffer = new unsigned char[rgb_size];

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            glm::u8vec3 color;
            getColorCell(x, y, bit_mask, color_mask, color);
            int position = (x * 3) + (((height - 1) - y) * (width * 3));
            new_buffer[position] = color.r;
            new_buffer[position + 1] = color.g;
            new_buffer[position + 2] = color.b;
        }
    }

    return new_buffer;
}

void Field::switchCell(int x, int y, unsigned char bit_mask, unsigned char color_mask)
{
    int position = (x + width * y) * 3;
    if (color_mask & R)
    {
        buffer[position] = buffer[position] | bit_mask;
    }

    if (color_mask & G)
    {
        buffer[position + 1] = buffer[position + 1] | bit_mask;
    }

    if (color_mask & B)
    {
        buffer[position + 2] = buffer[position + 2] | bit_mask;
    }
}

void Field::clearCell(int x, int y)
{
    int position = (x + width * y) * 3;
    buffer[position] = 0;
    buffer[position + 1] = 0;
    buffer[position + 2] = 0;
}
