#include "Field.h"
#include <utility>

Field::Field(const unsigned char * data, int width, int height)
    : width(width), height(height)
{
    int rgb_size = height * (width * 3);
    buffer = new unsigned char[rgb_size];
    for (int i = 0; i < rgb_size; ++i)
    {
        buffer[i] = data[i];
    }
}

Color* Field::getColorCell(int x, int y, unsigned char bit_mask, unsigned char color_mask)
{
    auto color = new Color();
    int position = (x * 3) + (y * (width * 3));

    if (color_mask & R)
    {
        color->red &= buffer[position] & bit_mask;
    }
    else
    {
        color->red = 0;
    }

    if (color_mask & G)
    {
        color->green &= buffer[position + 1] & bit_mask;
    }
    else
    {
        color->green = 0;
    }

    if (color_mask & B)
    {
        color->blue &= buffer[position + 2] & bit_mask;
    }
    else
    {
        color->blue = 0;
    }

    return color;
}

unsigned Field::getWidth()
{
    return width;
}

unsigned Field::getHeight()
{
    return height;
}

unsigned char* Field::getBuffer(unsigned char bit_mask, unsigned char color_mask)
{
    int rgb_size = height * (width * 3);
    auto new_buffer = new unsigned char[rgb_size];

    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            Color* color = getColorCell(x, y, bit_mask, color_mask);
            int position = (x * 3) + (((height - 1) - y) * (width * 3));
            new_buffer[position] = color->red;
            new_buffer[position + 1] = color->green;
            new_buffer[position + 2] = color->blue;
            delete color;
        }
    }
    return new_buffer;
}

void Field::switchCell(int x, int y, unsigned char bit_mask, unsigned char color_mask)
{
    int position = (x * 3) + (y * (width * 3));
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
    int position = (x * 3) + (y * (width * 3));
    buffer[position] = 0;
    buffer[position + 1] = 0;
    buffer[position + 2] = 0;
}
