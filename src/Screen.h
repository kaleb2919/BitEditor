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
    glm::vec2 position = glm::vec3(0);
    glm::vec3 color = glm::vec3(255);
    bool blink_state = true;
    float timer = 0.0f;
    float blink_delay = 0.5f;

    float getX() { return position.x; }
    float getY() { return position.y; }
    void setX(float x) { position.x = x; }
    void setY(float y) { position.y = y; }
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

    GLuint VAO, VBO, EBO;

    GLint width;
    GLint height;

    Field* field;
    Image* image;
    Shader* shader;

    Cursor cursor;

    unsigned char bit_mode = 0b11111111;
    unsigned char color_mode = RGB;
    bool colored_mode = true;

    void setup();
public:

    Screen(Image* image, Shader* shader);
    void draw();
    void renderField();

    void setBitMode(unsigned char new_bit_mode);
    void setColorMode(unsigned char new_color_mode);
    void moveCursor(int x, int y);
    void updateCursor(float delta_time);
    void set(int x, int y, glm::vec3 color);
    void setCellAtCursor();
    void clearCellAtCursor();
    void save(char const* file_name);
    void switchColoredMode();
};
