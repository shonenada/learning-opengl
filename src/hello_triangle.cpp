#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

const int WIDTH = 1920;
const int HEIGHT = 1080;
const char* TITLE = "OpenGL";

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void handleInput(GLFWwindow* window);

void errorCallback(int error, const char* description)
{
    std::cout << error << " " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(errorCallback);
}