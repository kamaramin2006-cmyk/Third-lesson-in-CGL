#include <iostream>
using namespace std;

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

const GLint SCR_WIDTH = 800;
const GLint SCR_HEIGHT = 600;

// ===== „ €Ì—«  «· Õﬂ„ =====
float colorValue = 0.0f;
float alphaValue = 1.0f;
float scaleValue = 1.0f;

// ===== Vertex Shader =====
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"uniform float xMove;\n"
"uniform float zMove;\n"
"uniform float scale;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4((aPos.x * scale) + xMove, aPos.y * scale, aPos.z + zMove, 1.0);\n"
"}\0";

// ===== Fragment Shader =====
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"uniform float colorShift;\n"
"uniform float alpha;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.2 + colorShift, 0.5, 0.8, alpha);\n"
"}\n\0";

// ===== Callbacks =====
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //  ﬂ»Ì—
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        scaleValue += 0.01f;

    //  ’€Ì—
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        scaleValue -= 0.01f;

    //  €ÌÌ— «··Ê‰
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        colorValue += 0.01f;

    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        colorValue -= 0.01f;

    // ‘›«›Ì…
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        alphaValue = 0.5f;
}

// ===== MAIN =====
int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "OpenGL HW", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glewExperimental = GL_TRUE;
    glewInit();

    // =====  ›⁄Ì· «·‘›«›Ì… =====
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // ===== Shaders =====
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ===== »Ì«‰«  „” ÿÌ· =====
    float vertices[] = {
        -0.3f, -0.3f, 0.0f,
         0.3f, -0.3f, 0.0f,
         0.3f,  0.3f, 0.0f,
        -0.3f,  0.3f, 0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // ===== Render Loop =====
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.1f, 0.15f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glUniform1f(glGetUniformLocation(shaderProgram, "colorShift"), colorValue);
        glUniform1f(glGetUniformLocation(shaderProgram, "alpha"), alphaValue);
        glUniform1f(glGetUniformLocation(shaderProgram, "scale"), scaleValue);

        glBindVertexArray(VAO);

        // ===== «·„” ÿÌ· «·√Ê· (Z ﬁ—Ì») =====
        glUniform1f(glGetUniformLocation(shaderProgram, "xMove"), -0.4f);
        glUniform1f(glGetUniformLocation(shaderProgram, "zMove"), 0.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // ===== «·„” ÿÌ· «·À«‰Ì (Z √»⁄œ) =====
        glUniform1f(glGetUniformLocation(shaderProgram, "xMove"), 0.4f);
        glUniform1f(glGetUniformLocation(shaderProgram, "zMove"), -0.5f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
