#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Shader.h"
#include "Image.h"
#include "Field.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec2 texture_coordinate;
};

struct Cursor
{
    glm::i32vec2 position = glm::i32vec2(0);
    glm::u8vec3 color = glm::u8vec3(255);

    bool blink_state = true;
    double timer = 0.0;
    double blink_delay = 0.5;

    int getX() { return position.x; }
    int getY() { return position.y; }
    void setX(int x) { position.x = x; }
    void setY(int y) { position.y = y; }
};

class Screen
{
    std::vector<Vertex> vertices = {
        {glm::vec3(1.0f, 1.0f, 0.0f), glm::vec2(1.0f, 1.0f)},
        {glm::vec3(1.0f, -1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
        {glm::vec3(-1.0f, -1.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
        {glm::vec3(-1.0f, 1.0f, 0.0f), glm::vec2(0.0f, 1.0f)}
    };

    std::vector<GLuint> indices = { 0, 1, 3, 1, 2, 3 };

    GLuint vao = 0;
    GLuint vbo = 0;
    GLuint ebo = 0;

    GLint width;
    GLint height;

    Field* field;
    Image* image;

    Cursor cursor;

    unsigned char bit_mode = 0b11111111;
    unsigned char color_mode = RGB;
    bool colored_mode = true;

    void setup();
public:

    Screen(Image* image);
    void draw();
    void renderField();

    void setBitMode(unsigned char new_bit_mode);
    void setColorMode(unsigned char new_color_mode);
    void moveCursor(double x, double y);
    void updateCursor(double delta_time);
    void setColor(int x, int y, glm::u8vec3 color);
    void setCellAtCursor();
    void clearCellAtCursor();
    void save(char const* file_name);
    void switchColoredMode();
};
