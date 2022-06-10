#include "Screen.h"

#include <bitset>
#include <iostream>

Screen::Screen(Image* image, Shader* shader)
    : width(image->getWidth()), height(image->getHeight()), image(image), shader(shader)
{
    field = new Field(image->getData(), width, height);
    setup();
}

void Screen::moveCursor(int x, int y)
{
    cursor.setX(x % width);
    cursor.setY(y % height);
}

void Screen::updateCursor(float delta_time)
{
    if (cursor.timer > 0)
    {
        cursor.timer -= delta_time;
    }
    else
    {
        cursor.timer = cursor.blink_delay;
        cursor.blink_state = !cursor.blink_state; 
    }
}

void Screen::setup()
{
    glCreateBuffers(1, &VBO);
    glNamedBufferStorage(VBO, static_cast<unsigned int>(vertices.size() * sizeof(Vertex)), &vertices[0], 0x0);

    glCreateBuffers(1, &EBO);
    glNamedBufferStorage(EBO, static_cast<unsigned int>(indices.size() * sizeof(unsigned int)), &indices[0], 0x0);

    glCreateVertexArrays(1, &VAO);
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(VAO, EBO);

    glEnableVertexArrayAttrib(VAO, 0);
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coordinate));
    glVertexArrayAttribBinding(VAO, 0, 0);
    glVertexArrayAttribBinding(VAO, 1, 0);
}

void Screen::draw()
{
    renderField();
    set(static_cast<int>(cursor.getX()), static_cast<int>(cursor.getY()), cursor.blink_state ? cursor.color : glm::vec3(128));
    image->reload();
    glBindTextureUnit(0, image->getDescriptor());
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(indices.size()), GL_UNSIGNED_INT, static_cast<void*>(nullptr));
    glBindVertexArray(0);
}

void Screen::renderField()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            auto color = field->getColorCell(x, y, bit_mode, color_mode);
            set(x, y, colored_mode? glm::vec3(color->red, color->green, color->blue)
                : color->isBlack(color_mode) ? glm::vec3(0) : glm::vec3(255));
            delete color;
        }
    }
}

void Screen::setBitMode(unsigned char new_bit_mode)
{
    bit_mode = new_bit_mode;
    std::cout << "Current bit mode: " << std::bitset<8>(new_bit_mode) << std::endl;
}

void Screen::setColorMode(unsigned char new_color_mode)
{
    color_mode = new_color_mode;
    std::cout << "Current bit mode: " << std::bitset<3>(new_color_mode) << std::endl;
}

void Screen::set(int x, int y, glm::vec3 color)
{
    int position = (x * 3) + (y * (width * 3));
    unsigned char* data = image->getData();
    data[position] = static_cast<unsigned char>(color.r);
    data[position+1] = static_cast<unsigned char>(color.g);
    data[position+2] = static_cast<unsigned char>(color.b);
}

void Screen::setCellAtCursor()
{
    field->switchCell(static_cast<int>(cursor.getX()), static_cast<int>(cursor.getY()), bit_mode, color_mode);
}

void Screen::clearCellAtCursor()
{
    field->clearCell(static_cast<int>(cursor.getX()), static_cast<int>(cursor.getY()));
}

void Screen::save(char const* file_name)
{
    Image::save(file_name, field->getWidth(), field->getHeight(), 3, field->getBuffer(0b11111111, RGB));
    std::cout << "Saved with name: " << file_name << std::endl;
}

void Screen::switchColoredMode()
{
    colored_mode = !colored_mode;
    std::cout << "Colored mode: " << colored_mode << std::endl;
}
