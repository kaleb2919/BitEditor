#include "Screen.h"

#include <bitset>
#include <iostream>

Screen::Screen(Image* image)
    : width(image->getWidth()), height(image->getHeight()), image(image)
{
    field = new Field(image->copyData(), image->getWidth(), image->getHeight());
    setup();
}

void Screen::moveCursor(double x, double y)
{
    cursor.setX(static_cast<int>(x));
    cursor.setY(static_cast<int>(y));
}

void Screen::updateCursor(double delta_time)
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
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, static_cast<unsigned int>(vertices.size() * sizeof(Vertex)), &vertices[0], 0x0);

    glCreateBuffers(1, &ebo);
    glNamedBufferStorage(ebo, static_cast<unsigned int>(indices.size() * sizeof(unsigned int)), &indices[0], 0x0);

    glCreateVertexArrays(1, &vao);
    glVertexArrayVertexBuffer(vao, 0, vbo, 0, sizeof(Vertex));
    glVertexArrayElementBuffer(vao, ebo);

    glEnableVertexArrayAttrib(vao, 0);
    glEnableVertexArrayAttrib(vao, 1);
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, position));
    glVertexArrayAttribFormat(vao, 1, 2, GL_FLOAT, GL_FALSE, offsetof(Vertex, texture_coordinate));
    glVertexArrayAttribBinding(vao, 0, 0);
    glVertexArrayAttribBinding(vao, 1, 0);
}

void Screen::draw()
{
    renderField();
    setColor(cursor.getX(), cursor.getY(), cursor.blink_state ? cursor.color : glm::u8vec3(128));
    image->reload();
    glBindTextureUnit(0, image->getDescriptor());
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, static_cast<GLint>(indices.size()), GL_UNSIGNED_INT, static_cast<void*>(nullptr));
    glBindVertexArray(0);
}

void Screen::renderField()
{
    for (int x = 0; x < width; x++)
    {
        for (int y = 0; y < height; y++)
        {
            glm::u8vec3 color;
            field->getColorCell(x, y, bit_mode, color_mode, color);
            if (colored_mode)
            {
                setColor(x, y, color);
            }
            else
            {
                if ((color.r == 0 || !(color_mode & R))
                    && (color.g == 0 || !(color_mode & G))
                    && (color.b == 0 || !(color_mode & B)))
                {
                    setColor(x, y, glm::u8vec3(0));
                }
                else
                {
                    setColor(x, y, glm::u8vec3(255));
                }
            }
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

void Screen::setColor(int x, int y, glm::u8vec3 color)
{
    image->setColor(x, y, color);
}

void Screen::setCellAtCursor()
{
    field->switchCell(cursor.getX(), cursor.getY(), bit_mode, color_mode);
}

void Screen::clearCellAtCursor()
{
    field->clearCell(cursor.getX(), cursor.getY());
}

void Screen::save(char const* file_name)
{
    auto buffer = field->copyBuffer(0b11111111, RGB);
    Image::save(file_name, field->getWidth(), field->getHeight(), 3, buffer);
    std::cout << "Saved with name: " << file_name << std::endl;
    delete buffer;
}

void Screen::switchColoredMode()
{
    colored_mode = !colored_mode;
    std::cout << "Colored mode: " << colored_mode << std::endl;
}
