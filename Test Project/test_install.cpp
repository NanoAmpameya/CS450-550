#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>  // Basic include for GLM
#include <SOIL2/SOIL2.h>  // For SOIL2
#include <iostream>

void init(GLFWwindow* window) {}

void display(GLFWwindow* window, double currentTime) {
    glClearColor(0.84f, 0.25f, 0.03f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); //Needed for MacOS`
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Test", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;  // Ensures all extensions are loaded including Core Profiles
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glfwSwapInterval(1);  // enables Vsync3

    init(window);

    // Test GLM
    glm::vec3 vec(1.0f, 0.0f, 0.0f);
    std::cout << "GLM vector: " << vec.x << std::endl;

    // Test SOIL2 (load a dummy image; replace with a real path for full test)
    int width, height;
    unsigned char* image = SOIL_load_image("img_mars.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (!image) {
        std::cerr << "SOIL2 failed to load image" << std::endl;
    }
    else {
        std::cout << "SOIL2 image loaded: " << width << "x" << height << std::endl;
        SOIL_free_image_data(image);
    }

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}