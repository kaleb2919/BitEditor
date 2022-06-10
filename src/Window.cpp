#include "Window.h"

#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

bool Window::hasBeenNotified(GLenum key_code)
{
    bool is_pressed = std::any_of(pressed_keys.begin(), pressed_keys.end(),
                                  [key_code](GLenum key_pressed) { return key_pressed == key_code; });
    if (!is_pressed)
    {
        pressed_keys.push_back(key_code);
    }

    return is_pressed;
}

Window::Window(int screen_width, int screen_height, const char* title) :
    width(screen_width), height(screen_height)
{
    if (!isInitiated())
    {
        Init();
    }

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });
    glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    });
}

void Window::processInput()
{
    pressed_keys.remove_if([this](GLenum pressed_key) { return isKeyRelease(pressed_key) && isMouseRelease(pressed_key); });

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

bool Window::isMousePress(GLenum key_code)
{
    return glfwGetMouseButton(window, key_code) == GLFW_PRESS && !hasBeenNotified(key_code);
}

bool Window::isMousePressed(GLenum key_code)
{
    return glfwGetMouseButton(window, key_code) == GLFW_PRESS;
}

bool Window::isMouseRelease(GLenum key_code)
{
    return glfwGetMouseButton(window, key_code) == GLFW_RELEASE;
}

bool Window::isKeyPress(GLenum key_code)
{
    return glfwGetKey(window, key_code) == GLFW_PRESS && !hasBeenNotified(key_code);
}

bool Window::isKeyPressed(GLenum key_code)
{
    return glfwGetKey(window, key_code) == GLFW_PRESS;
}

bool Window::isKeyRelease(GLenum key_code)
{
    return glfwGetKey(window, key_code) == GLFW_RELEASE;
}

bool Window::isRunning()
{
    return !glfwWindowShouldClose(window);
}

bool Window::isInitiated()
{
    return is_initiated;
}

void Window::Init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    is_initiated = true;
}

int Window::getWidth()
{
    return width;
}

int Window::getHeight()
{
    return height;
}

int Window::getKey(int key_code)
{
    return glfwGetKey(window, key_code);
}

void Window::close()
{
    glfwSetWindowShouldClose(window, true);
}

double Window::getTime()
{
    return glfwGetTime();
}

float Window::getDeltaTime()
{
    current_time = getTime();
    auto delta_time = current_time - last_time;
    last_time = current_time;
    return delta_time;
}

void Window::setWindowSizeCallback(void(* resize)(GLFWwindow* window, int width, int height))
{
    glfwSetWindowSizeCallback(window, resize);
}

void Window::clear()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Window::swap()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void Window::getMousePosition(double& x, double& y)
{
    glfwGetCursorPos(window, &x, &y);
}

void Window::getMoiseOffset(double& x, double& y)
{
    double curr_x, curr_y;
    glfwGetCursorPos(window, &curr_x, &curr_y);
    x = (curr_x - prev_x) * (inverse_mouse_x ? -1 : 1);
    y = (curr_y - prev_y) * (inverse_mouse_y ? -1 : 1);
    prev_x = curr_x;
    prev_y = curr_y;
}

void Window::getCurrentResolution(int& width_res, int& height_res)
{
    if (!isInitiated())
    {
        Init();
    }

    auto monitor = glfwGetPrimaryMonitor();
    auto mode = glfwGetVideoMode(monitor);
    width_res = mode->width;
    height_res = mode->height;
}
