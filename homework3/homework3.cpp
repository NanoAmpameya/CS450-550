#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <iostream>
#include <string>
#include <fstream>

#define numVAOs 2
#define numVBOs 4

GLuint renderingProgram;
GLuint vao[numVAOs];
GLuint vbo[numVBOs];
GLint mvLoc, projLoc;

const int cubeVertCount = 36;
const int pyramidVertCount = 18;

// Original cube (not copied from course materials). 6 faces, 2 triangles each.
// Each face gets one color so the 3D shape reads without lighting.
float cubeVertices[] = {
    // front
    -0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,   0.5f,  0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
    // back
     0.5f, -0.5f, -0.5f,  -0.5f, -0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
    // left
    -0.5f, -0.5f, -0.5f,  -0.5f, -0.5f,  0.5f,  -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,  -0.5f,  0.5f,  0.5f,  -0.5f,  0.5f, -0.5f,
    // right
     0.5f, -0.5f,  0.5f,   0.5f, -0.5f, -0.5f,   0.5f,  0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,   0.5f,  0.5f, -0.5f,   0.5f,  0.5f,  0.5f,
    // top
    -0.5f,  0.5f,  0.5f,   0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,   0.5f,  0.5f, -0.5f,  -0.5f,  0.5f, -0.5f,
    // bottom
    -0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,   0.5f, -0.5f,  0.5f,  -0.5f, -0.5f,  0.5f
};

float cubeColors[] = {
    // front - coral
    0.91f, 0.34f, 0.40f,  0.91f, 0.34f, 0.40f,  0.91f, 0.34f, 0.40f,
    0.91f, 0.34f, 0.40f,  0.91f, 0.34f, 0.40f,  0.91f, 0.34f, 0.40f,
    // back - teal
    0.18f, 0.72f, 0.67f,  0.18f, 0.72f, 0.67f,  0.18f, 0.72f, 0.67f,
    0.18f, 0.72f, 0.67f,  0.18f, 0.72f, 0.67f,  0.18f, 0.72f, 0.67f,
    // left - gold
    0.96f, 0.75f, 0.18f,  0.96f, 0.75f, 0.18f,  0.96f, 0.75f, 0.18f,
    0.96f, 0.75f, 0.18f,  0.96f, 0.75f, 0.18f,  0.96f, 0.75f, 0.18f,
    // right - violet
    0.61f, 0.35f, 0.81f,  0.61f, 0.35f, 0.81f,  0.61f, 0.35f, 0.81f,
    0.61f, 0.35f, 0.81f,  0.61f, 0.35f, 0.81f,  0.61f, 0.35f, 0.81f,
    // top - sky
    0.35f, 0.62f, 0.93f,  0.35f, 0.62f, 0.93f,  0.35f, 0.62f, 0.93f,
    0.35f, 0.62f, 0.93f,  0.35f, 0.62f, 0.93f,  0.35f, 0.62f, 0.93f,
    // bottom - lime
    0.45f, 0.80f, 0.22f,  0.45f, 0.80f, 0.22f,  0.45f, 0.80f, 0.22f,
    0.45f, 0.80f, 0.22f,  0.45f, 0.80f, 0.22f,  0.45f, 0.80f, 0.22f
};

// Original square pyramid: 4 sides + 2-triangle base.
float pyramidVertices[] = {
    // +Z face
     0.0f,  0.75f,  0.0f,  -0.6f, -0.5f,  0.6f,   0.6f, -0.5f,  0.6f,
    // +X face
     0.0f,  0.75f,  0.0f,   0.6f, -0.5f,  0.6f,   0.6f, -0.5f, -0.6f,
    // -Z face
     0.0f,  0.75f,  0.0f,   0.6f, -0.5f, -0.6f,  -0.6f, -0.5f, -0.6f,
    // -X face
     0.0f,  0.75f,  0.0f,  -0.6f, -0.5f, -0.6f,  -0.6f, -0.5f,  0.6f,
    // base
    -0.6f, -0.5f,  0.6f,  -0.6f, -0.5f, -0.6f,   0.6f, -0.5f, -0.6f,
    -0.6f, -0.5f,  0.6f,   0.6f, -0.5f, -0.6f,   0.6f, -0.5f,  0.6f
};

float pyramidColors[] = {
    0.95f, 0.45f, 0.20f,  0.95f, 0.45f, 0.20f,  0.95f, 0.45f, 0.20f,
    0.20f, 0.78f, 0.55f,  0.20f, 0.78f, 0.55f,  0.20f, 0.78f, 0.55f,
    0.25f, 0.48f, 0.90f,  0.25f, 0.48f, 0.90f,  0.25f, 0.48f, 0.90f,
    0.90f, 0.82f, 0.20f,  0.90f, 0.82f, 0.20f,  0.90f, 0.82f, 0.20f,
    0.55f, 0.55f, 0.58f,  0.55f, 0.55f, 0.58f,  0.55f, 0.55f, 0.58f,
    0.55f, 0.55f, 0.58f,  0.55f, 0.55f, 0.58f,  0.55f, 0.55f, 0.58f
};

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

void setupObject(int vaoIndex, int posVbo, int colorVbo,
                 float *positions, int posBytes,
                 float *colors, int colorBytes) {
    glBindVertexArray(vao[vaoIndex]);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[posVbo]);
    glBufferData(GL_ARRAY_BUFFER, posBytes, positions, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vbo[colorVbo]);
    glBufferData(GL_ARRAY_BUFFER, colorBytes, colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void init(GLFWwindow* window) {
    renderingProgram = buildShaderProgram();
    mvLoc = glGetUniformLocation(renderingProgram, "mv_matrix");
    projLoc = glGetUniformLocation(renderingProgram, "proj_matrix");

    glGenVertexArrays(numVAOs, vao);
    glGenBuffers(numVBOs, vbo);

    // Object 0: cube  → VAO 0, VBO 0 positions, VBO 1 colors
    setupObject(0, 0, 1,
                cubeVertices, sizeof(cubeVertices),
                cubeColors, sizeof(cubeColors));

    // Object 1: pyramid → VAO 1, VBO 2 positions, VBO 3 colors
    setupObject(1, 2, 3,
                pyramidVertices, sizeof(pyramidVertices),
                pyramidColors, sizeof(pyramidColors));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glClearColor(0.08f, 0.10f, 0.16f, 1.0f);
}

void display(GLFWwindow* window, double currentTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glUseProgram(renderingProgram);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    float aspect = (height == 0) ? 1.0f : (float)width / (float)height;

    glm::mat4 pMat = glm::perspective(glm::radians(60.0f), aspect, 0.1f, 100.0f);
    glm::mat4 vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -6.0f));

    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(pMat));

    float t = (float)currentTime;

    // Cube: translate off origin + scale, then spin around Y (its own animation)
    glm::mat4 cubeM = glm::translate(glm::mat4(1.0f), glm::vec3(-1.6f, 0.0f, 0.0f));
    cubeM = glm::scale(cubeM, glm::vec3(1.15f, 1.15f, 1.15f));
    cubeM = glm::rotate(cubeM, t * 0.9f, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 cubeMV = vMat * cubeM;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(cubeMV));
    glBindVertexArray(vao[0]);
    glDrawArrays(GL_TRIANGLES, 0, cubeVertCount);

    // Pyramid: translate off origin + scale, then bob and spin on Z (different math)
    float bob = 0.35f * sinf(t * 1.7f);
    glm::mat4 pyrM = glm::translate(glm::mat4(1.0f), glm::vec3(1.7f, bob, 0.0f));
    pyrM = glm::scale(pyrM, glm::vec3(0.95f, 1.25f, 0.95f));
    pyrM = glm::rotate(pyrM, t * 1.4f, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::mat4 pyrMV = vMat * pyrM;
    glUniformMatrix4fv(mvLoc, 1, GL_FALSE, glm::value_ptr(pyrMV));
    glBindVertexArray(vao[1]);
    glDrawArrays(GL_TRIANGLES, 0, pyramidVertCount);

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
    GLFWwindow* window = glfwCreateWindow(800, 600, "Homework 3 - 3D Scene", NULL, NULL);
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
