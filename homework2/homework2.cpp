#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <SOIL2/SOIL2.h>
#include <iostream>
#include <string>
#include <fstream>

#define numVAOs 1
#define numVBOs 1

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLint uTimeLoc;

std::string loadShaderSource(const char *filePath) {
    std::string source;
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open()) {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
        return "";
    } else {
        std::cout << "Loading shader: " << filePath << std::endl;
    }

    std::string line;
    while (std::getline(fileStream, line)) {
        source.append(line + "\n");
    }

    fileStream.close();
    return source;
}

void printShaderLog(GLuint shader) {
    int len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log(len, '\0');
        glGetShaderInfoLog(shader, len, NULL, &log[0]);
        std::cerr << log << std::endl;
    }
}

void printProgramLog(GLuint program) {
    int len = 0;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
    if (len > 1) {
        std::string log(len, '\0');
        glGetProgramInfoLog(program, len, NULL, &log[0]);
        std::cerr << log << std::endl;
    }
}

GLuint buildShaderProgram() {
    GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);

    std::string vertShaderStr = loadShaderSource("shader.vert");
    std::string fragShaderStr = loadShaderSource("shader.frag");

    const char *vertShaderSrc = vertShaderStr.c_str();
    const char *fragShaderSrc = fragShaderStr.c_str();

    glShaderSource(vShader, 1, &vertShaderSrc, NULL);
    glShaderSource(fShader, 1, &fragShaderSrc, NULL);

    glCompileShader(vShader);
    GLint compiled = 0;
    glGetShaderiv(vShader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        std::cerr << "Vertex shader failed to compile:" << std::endl;
        printShaderLog(vShader);
    }

    glCompileShader(fShader);
    glGetShaderiv(fShader, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        std::cerr << "Fragment shader failed to compile:" << std::endl;
        printShaderLog(fShader);
    }

    GLuint vfProgram = glCreateProgram();
    glAttachShader(vfProgram, vShader);
    glAttachShader(vfProgram, fShader);
    glLinkProgram(vfProgram);

    GLint linked = 0;
    glGetProgramiv(vfProgram, GL_LINK_STATUS, &linked);
    if (linked != GL_TRUE) {
        std::cerr << "Shader program failed to link:" << std::endl;
        printProgramLog(vfProgram);
    }

    return vfProgram;
}

void init(GLFWwindow* window) {
    renderingProgram = buildShaderProgram();
    uTimeLoc = glGetUniformLocation(renderingProgram, "uTime");

    // Original sailboat: 2 triangles (6 vertices). Each vertex is
    // x, y, z, triangleId  — not the course-materials triangle.
    float verts[] = {
        // Sail (triangle 0)
         0.00f,  0.55f, 0.0f, 0.0f,
        -0.22f, -0.05f, 0.0f, 0.0f,
         0.02f, -0.05f, 0.0f, 0.0f,
        // Hull (triangle 1)
        -0.48f, -0.08f, 0.0f, 1.0f,
         0.48f, -0.08f, 0.0f, 1.0f,
         0.00f, -0.42f, 0.0f, 1.0f
    };

    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);

    glBindVertexArray(vao[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

    // location 0: position (vec3)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // location 1: triangle id (float) so the shader can animate each triangle differently
    glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glClearColor(0.12f, 0.40f, 0.68f, 1.0f);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(renderingProgram);
    glUniform1f(uTimeLoc, (float)currentTime);
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

int main() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "Homework 2 - Sailboat", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        return -1;
    }

    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    glfwSwapInterval(1);

    init(window);

    while (!glfwWindowShouldClose(window)) {
        display(window, glfwGetTime());
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
