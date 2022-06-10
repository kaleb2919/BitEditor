#pragma once

enum EColor
{
    R = 0b00000001,
    G = 0b00000010,
    B = 0b00000100,
    RGB = 0b00000111,
};

struct Color
{
    unsigned char red = 0b11111111;
    unsigned char green = 0b11111111;
    unsigned char blue = 0b11111111;

    bool isBlack(unsigned char color_mode)
    {
        return (red == 0 || !(color_mode & R)) && (green == 0 || !(color_mode & G)) && (blue == 0 || !(color_mode & B));
    }
};

class Field
{
    int width;
    int height;

    unsigned char* buffer;

public:

    Field(const unsigned char * data, int width, int height);

    unsigned int getWidth();
    unsigned int getHeight();

    Color* getColorCell(int x, int y, unsigned char bit_mask, unsigned char color_mask);
    unsigned char* getBuffer(unsigned char bit_mask, unsigned char color_mask);
    void switchCell(int x, int y, unsigned char bit_mask, unsigned char color_mask);
    void clearCell(int x, int y);
};
