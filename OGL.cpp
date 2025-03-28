#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

// ������� ��� �������� � ���������� �������
GLuint CompileShader(const char* source, GLenum type) {
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return shader;
}

// ������� ��� �������� ��������� ���������
GLuint CreateShaderProgram(const char* vertexShaderSource, const char* fragmentShaderSource) {
    GLuint vertexShader = CompileShader(vertexShaderSource, GL_VERTEX_SHADER);
    GLuint fragmentShader = CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    GLint success;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main() {
    // ������������� GLFW
    if (!glfwInit()) {
        std::cerr << "ERROR::GLFW::INIT_FAILED" << std::endl;
        return -1;
    }

    // �������� ���� � ���������� OpenGL
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL ES Emulator", nullptr, nullptr);
    if (!window) {
        std::cerr << "ERROR::GLFW::WINDOW_CREATION_FAILED" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // �������� ������������ �������������

    // ������������� GLEW
    if (glewInit() != GLEW_OK) {
        std::cerr << "ERROR::GLEW::INIT_FAILED" << std::endl;
        return -1;
    }

    // ����������� ������ �������������
    GLfloat vertices[] = {
        // ������ ����������� (�����)
        -1.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, 0.0f,
         0.5f,  0.0f, 0.0f,

         // ������ ����������� (�����)
          1.0f,  0.0f, 0.0f,
          0.5f,  0.5f, 0.0f,
         -0.5f,  0.0f, 0.0f,

         // ������ ����������� (�����)
         -0.5f,  0.5f, 0.0f,
          0.5f,  0.5f, 0.0f,
          0.0f,  1.0f, 0.0f
    };

    // �������� VAO, VBO
    GLuint VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // �������� ��� ��������
    const char* vertexShaderSource = R"(
        #version 130
        in vec3 position;
        void main() {
            gl_Position = vec4(position, 1.0);
        }
    )";

    const char* fragmentShaderSource = R"(
        #version 130
        out vec4 color;
        void main() {
            color = vec4(1.0f, 1.0f, 0.0f, 1.0f); // Ƹ���� ����
        }
    )";

    // �������� ��������� ���������
    GLuint shaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    // ������� ����
    while (!glfwWindowShouldClose(window)) {
        // ��������� �������
        glfwPollEvents();

        // ������� ������
        glClear(GL_COLOR_BUFFER_BIT);

        // ������ ������������
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);  // ������ �����������
        glDrawArrays(GL_TRIANGLES, 3, 3);  // ������ �����������
        glDrawArrays(GL_TRIANGLES, 6, 3);  // ������ �����������
        glBindVertexArray(0);

        // ���������� ������
        glfwSwapBuffers(window);
    }

    // ������������ ��������
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

