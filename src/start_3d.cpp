#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cmath>

#include "ofs/shader.h"

const int WIDTH = 1920;
const int HEIGHT = 1080;
const char* TITLE = "OpenGL - 3D";

void frameBufferSizeCallback(GLFWwindow* window, int width, int height);
void handleInput(GLFWwindow* window, unsigned int shaderProgram, int VAO, int EBO);
void updateUniformColor(int shaderProgram);

void errorCallback(int error, const char* description) {
    std::cout << error << " " << description << std::endl;
}

/*
 * Scale + Translation
 * {
 *   S1, 0,  0,  T1,
 *   0,  S2, 0,  T2,
 *   0,  0,  S3, T3,
 *   0,  0,  0,  1
 *  }
 */

/*
 * Rotation around X-axis
 * {
 *  1, 0,    0,     0,
 *  0, cosX, -sinX, 0,
 *  0, sinX, cosX,  0,
 *  0, 0,    0,     1
 * }
 */

/*
 * Rotation around Y-axis
 * {
 *  cosY, 0, sinY, 0,
 *  0, 1, 0, 0,
 *  -sinY, 0, cosY, 0,
 *  0, 0, 0, 1
 * }
 */

/*
 * Rotation around Z-axis
 * {
 *   cosZ, -sinZ, 0, 0,
 *   sinZ, cosZ, 0, 0,
 *   0, 0, 1, 0,
 *   0, 0, 0, 1
 * }
 */

glm::mat4 get_translation_mat(glm::vec3 vec) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::translate(trans, vec);
    return trans;
}

glm::vec4 translation(glm::vec4 obj, glm::vec3 vec) {
    return obj * get_translation_mat(vec);
}

glm::mat4 get_rotate_mat(float angle, glm::vec3 vec) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::rotate(trans, glm::radians(angle), vec);
    return trans;
}

glm::vec4 rotate(glm::vec4 obj, float angle, glm::vec3 rotate) {
    return obj * get_rotate_mat(angle, rotate);
}

glm::mat4 get_scale_mat(glm::vec3 vec) {
    glm::mat4 trans = glm::mat4(1.0f);
    trans = glm::scale(trans, vec);
    return trans;
}

glm::vec4 scale(glm::vec4 obj, glm::vec3 vec) {
    return obj * get_scale_mat(vec);
}

unsigned int loadTexture(const char* path, GLenum format) {
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);

    return texture;
}

int main() {
    glfwSetErrorCallback(errorCallback);

    int initOk = glfwInit();
    if (!initOk) {
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, TITLE, NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, frameBufferSizeCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to init GLAD" << std::endl;
        return 1;
    }

    stbi_set_flip_vertically_on_load(true);

    unsigned int texture1 = loadTexture("../resources/wood.jpg", GL_RGB);
    unsigned int texture2 = loadTexture("../resources/awesomeface.png", GL_RGBA);

    // position(x, y, z), texture(s, t)
    float vertices[] = {
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
            0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
            0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    float texCoords[] = {
            0.0f, 0.0f, // lower-left
            1.0, 0.0f, // lower-right
            0.5f, 1.0f // top-center
    };
    unsigned int indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    Shader shader("../shader/3d/vertex.glsl", "../shader/3d/fragment.glsl");

    unsigned int VAO, VBO;
//    unsigned int EBO;
    // vertex array object
    glGenVertexArrays(1, &VAO);
    // vertex buffer objects
    glGenBuffers(1, &VBO);
    // Element buffer object
//    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Any buffer call will be used to VBO from this point.
    // GL_STATIC_DRAW: data is set once, used many times.
    // GL_DYNAMIC_DRAW: data is changed a lot, used many times.
    // GL_STREAM_DRAW: data is set only once, used by the GPU at most a few times.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

//    glBindBuffer(GL_ARRAY_BUFFER, 0);
//    glBindVertexArray(0);

    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        shader.use();
        shader.set4Float("color", 0.0f, greenValue, 0.0f, 1.0f);
        shader.setInt("texture1", 0);
        shader.setInt("texture2", 1);
        glm::mat4 trans = glm::mat4(1.0f);
//        trans = glm::scale(trans, glm::vec3(1.5f, 1.5f, 1.5f));
        trans = glm::rotate(trans, (float) glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));
//        trans = glm::rotate(trans, glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        shader.setMat4fv("transform", trans);

        // V[clip] = M[projection] * M[view] * M[model] * V[local]
        glm::mat4 model = glm::mat4(1.0f);
//        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, (float) glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        shader.setMat4fv("model", model);

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0, -3.0f));
        shader.setMat4fv("view", view);

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        shader.setMat4fv("projection", projection);


        glBindVertexArray(VAO);
//        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shader.ID);

    glfwTerminate();
    return 0;
}

void frameBufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void handleInput(GLFWwindow *window, unsigned int shaderProgram, int VAO, int EBO) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
}
