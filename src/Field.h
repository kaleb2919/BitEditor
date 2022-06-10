#pragma once
#include <glm/vec3.hpp>

enum EColor
{
    R = 0b00000001,
    G = 0b00000010,
    B = 0b00000100,
    RGB = 0b00000111,
};

class Field
{
    int width;
    int height;

    unsigned char* buffer;

public:

    Field(unsigned char * data, int width, int height);

    int getWidth();
    int getHeight();

    void getColorCell(int x, int y, unsigned char bit_mask, unsigned char color_mask, glm::u8vec3 &color);
    unsigned char* copyBuffer(unsigned char bit_mask, unsigned char color_mask);
    void switchCell(int x, int y, unsigned char bit_mask, unsigned char color_mask);
    void clearCell(int x, int y);
};
