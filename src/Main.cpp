#include <iostream>
#include <filesystem>

#include "Screen.h"
#include "Window.h"
#include "Shader.h"

void printHelp()
{
    std::cout << "##################################" << std::endl;
    std::cout << "#                                #" << std::endl;
    std::cout << "#  Space - fill cell             #" << std::endl;
    std::cout << "#  Delete - clear cell           #" << std::endl;
    std::cout << "#  S - save image                #" << std::endl;
    std::cout << "#  H - print this message        #" << std::endl;
    std::cout << "#  F1 - switch Red bit color     #" << std::endl;
    std::cout << "#  F2 - switch Green bit color   #" << std::endl;
    std::cout << "#  F3 - switch Blue bit color    #" << std::endl;
    std::cout << "#  F4 - switch mono-color mode   #" << std::endl;
    std::cout << "#  F5 - reset color to RGB       #" << std::endl;
    std::cout << "#  1..8 - switch color bit       #" << std::endl;
    std::cout << "#                                #" << std::endl;
    std::cout << "##################################" << std::endl;
}

bool EndsWith(std::string const &str, std::string const &suffix)
{
    if (str.size() < suffix.size()) { return false; }

    return 0 == str.compare(str.size() - suffix.size(),
                            suffix.size(),
                            suffix,
                            0,
                            suffix.size());
}

int window_width = 720;
int window_height = 720;

void resize(GLFWwindow* window, int width, int height)
{
    window_width = width;
    window_height = height;
}

int main()
{
    Window* window = nullptr;

    int input = 0;
    std::string file_name;
    std::cout << "1) New image" << std::endl;
    std::cout << "2) Open image" << std::endl;

    int image_width = 16;
    int image_height = 8;

    Image* image = nullptr;

    bool is_correct_input = false;
    while (!is_correct_input)
    {
        std::cout << "Input: ";
        std::cin >> input;

        switch (input)
        {
        case 1:
            std::cout << "File name: ";
            std::cin >> file_name;

            if (!EndsWith(file_name, ".png"))
            {
                file_name.append(".png");
            }

            while (!is_correct_input)
            {
                std::cout << "Width (px): ";
                std::cin >> image_width;

                if (image_width > 0)
                {
                    is_correct_input = true;
                }
                else
                {
                    std::cout << "Wrong width" << std::endl;
                }
            }
            is_correct_input = false;
            while (!is_correct_input)
            {
                std::cout << "Height (px): ";
                std::cin >> image_height;

                if (image_height > 0)
                {
                    is_correct_input = true;
                }
                else
                {
                    std::cout << "Wrong height" << std::endl;
                }
            }

            image = new Image(image_width, image_height, GL_RGB8);

            break;
        case 2:
            std::cout << "File name: ";
            std::cin >> file_name;

            if (!EndsWith(file_name, ".png"))
            {
                file_name.append(".png");
            }

            if (std::filesystem::exists(file_name))
            {
                image = new Image(file_name.c_str());
                image_width = image->getWidth();
                image_height = image->getHeight();
            }
            else
            {
                is_correct_input = true;
                std::cout << "File not found" << std::endl;
                break;
            }

            is_correct_input = true;

            break;
        default:
            std::cout << "Wrong command" << std::endl;
        }
    }

    if (!image && !window)
    {
        return -1;
    }

    int resolution_width, resolution_height;
    Window::getCurrentResolution(resolution_width,resolution_height);

    resolution_width *= 0.8f;
    resolution_height *= 0.8f;

    if (resolution_height > resolution_width && resolution_width >= image_width)
    {
        window_width = (resolution_width - (resolution_width % image_width));
        window_height = (image_height * (resolution_width / image_width));
    }
    else if (resolution_width > resolution_height && resolution_height >= image_height)
    {
        window_height = (resolution_height - (resolution_height % image_height));
        window_width =  (image_width * (resolution_height / image_height));
    }
    else
    {
        window_width = image_width;
        window_height = image_height;
    }
    
    std::cout << "Initiated with width:" << window_width << " height:" << window_height << std::endl;

    window = new Window(window_width, window_height, "BitEditor");
    window->setWindowSizeCallback(resize);

    image->init();

    auto shader = new Shader();
    if (!shader->loadSrcShaders(
        "#version 460 core\n layout (location = 0) in vec3 in_position;\n layout (location = 1) in vec2 in_tex_coord;\n out vec2 f_tex_coord;\n void main() { f_tex_coord = in_tex_coord; gl_Position = vec4(in_position, 1.0); }",
        "#version 460 core\n out vec4 FragColor;\n in vec2 f_tex_coord;\n uniform sampler2D u_texture;\n void main() { FragColor = texture(u_texture, f_tex_coord); }"
    ))
    {
        std::cout << "Failed to load shader" << std::endl;
        return 1;
    }

    shader->setInt("u_texture", 0);
    shader->use();

    printHelp();

    auto screen = Screen(image);

    double cursor_x = 0;
    double cursor_y = 0;

    unsigned char bit_mode = 0b11111111;
    unsigned char color_mode = RGB;

    while (window->isRunning())
    {
        window->processInput();
        window->clear();
        double delta_time = window->getDeltaTime();
        double delta_move = 10 * delta_time;

        if (window->isKeyPressed(KEY_UP))
        {
            cursor_y += delta_move;
            cursor_y = cursor_y > image_height ? 0 : cursor_y;
            screen.moveCursor(cursor_x, cursor_y);
        }
        if (window->isKeyPressed(KEY_DOWN))
        {
            cursor_y -= delta_move;
            cursor_y = cursor_y < 0 ? image_height - 1 : cursor_y;
            screen.moveCursor(cursor_x, cursor_y);
        }
        if (window->isKeyPressed(KEY_RIGHT))
        {
            cursor_x += delta_move;
            cursor_x = cursor_x > image_width ? 0 : cursor_x;
            screen.moveCursor(cursor_x, cursor_y);
        }
        if (window->isKeyPressed(KEY_LEFT))
        {
            cursor_x -= delta_move;
            cursor_x = cursor_x < 0 ? image_width - 1 : cursor_x;
            screen.moveCursor(cursor_x, cursor_y);
        }
        if (window->isKeyPress(KEY_SPACE))
        {
            screen.setCellAtCursor();
        }
        if (window->isKeyPress(KEY_DELETE))
        {
            screen.clearCellAtCursor();
        }
        if (window->isKeyPress(KEY_S))
        {
            screen.save(file_name.c_str());
        }
        if (window->isKeyPress(KEY_H))
        {
            printHelp();
        }
        if (window->isKeyPress(KEY_F1))
        {
            color_mode ^= R;
            screen.setColorMode(color_mode);
        }
        if (window->isKeyPress(KEY_F2))
        {
            color_mode ^= G;
            screen.setColorMode(color_mode);
        }
        if (window->isKeyPress(KEY_F3))
        {
            color_mode ^= B;
            screen.setColorMode(color_mode);
        }
        if (window->isKeyPress(KEY_F4))
        {
            screen.switchColoredMode();
        }
        if (window->isKeyPress(KEY_F5))
        {
            color_mode = RGB;
            screen.setColorMode(color_mode);
        }
        if (window->isKeyPress(KEY_1))
        {
            bit_mode ^= 0b00000001;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_2))
        {
            bit_mode ^= 0b00000010;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_3))
        {
            bit_mode ^= 0b00000100;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_4))
        {
            bit_mode ^= 0b00001000;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_5))
        {
            bit_mode ^= 0b00010000;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_6))
        {
            bit_mode ^= 0b00100000;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_7))
        {
            bit_mode ^= 0b01000000;
            screen.setBitMode(bit_mode);
        }
        if (window->isKeyPress(KEY_8))
        {
            bit_mode ^= 0b10000000;
            screen.setBitMode(bit_mode);
        }
        if (window->isMousePressed(MOUSE_BUTTON_LEFT))
        {
            double x,y;
            window->getMousePosition(x, y);
            x = std::clamp<double>(x, 1, window_width - 1);
            y = std::clamp<double>(y, 1, window_height - 1);
            cursor_x = floor(image_width * (x / window_width));
            cursor_y = floor(image_height * ((window_height - y) / window_height));
            screen.moveCursor(cursor_x, cursor_y);
            screen.setCellAtCursor();
        }
        if (window->isMousePressed(MOUSE_BUTTON_RIGHT))
        {
            double x,y;
            window->getMousePosition(x, y);
            x = std::clamp<double>(x, 1, window_width - 1);
            y = std::clamp<double>(y, 1, window_height - 1);
            cursor_x = floor(image_width * (x / window_width));
            cursor_y = floor(image_height * ((window_height - y) / window_height));
            screen.moveCursor(cursor_x, cursor_y);
            screen.clearCellAtCursor();
        }

        screen.updateCursor(delta_time);
        screen.draw();
        window->swap();
    }
}
