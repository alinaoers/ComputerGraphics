//
// Created by Алина Орешина on 22/03/2022.
//

#include <cstdio>
#include <GL/glew.h>
#include <glfw3.h>

GLFWwindow *window;

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;

#include <common/shader.hpp>

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(1024, 768, "Tutorial 04 - Colored Cube", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    GLuint programID = LoadShaders("TransformVertexShader.vertexshader", "ColorFragmentShader.fragmentshader");

    GLuint MatrixID = glGetUniformLocation(programID, "MVP");

    GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
    GLuint vertexColorID = glGetAttribLocation(programID, "vertexColor");

    static const GLfloat g_vertex_buffer_data[] = {
            0.0f, 0.0f, 0.45f,
            0.45f, 0.0f, 0.0f,
            0.0f, 0.9f, 0.0f,

            0.0f, 0.0f, 0.45f,
            -0.45f, 0.0f, 0.0f,
            0.0f, 0.9f, 0.0f,

            0.0f, 0.0f, -0.45f,
            0.45f, 0.0f, 0.0f,
            0.0f, 0.9f, 0.0f,

            0.0f, 0.0f, -0.45f,
            -0.45f, 0.0f, 0.0f,
            0.0f, 0.9f, 0.0f,

            0.0f, 0.0f, 0.45f,
            0.45f, 0.0f, 0.0f,
            0.0f, -0.9f, 0.0f,

            0.0f, 0.0f, 0.45f,
            -0.45f, 0.0f, 0.0f,
            0.0f, -0.9f, 0.0f,

            0.0f, 0.0f, -0.45f,
            0.45f, 0.0f, 0.0f,
            0.0f, -0.9f, 0.0f,

            0.0f, 0.0f, -0.45f,
            -0.45f, 0.0f, 0.0f,
            0.0f, -0.9f, 0.0f,
    };

    static const GLfloat g_color_buffer_data[] = {
            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.3f, 0.5f, 0.9f,

            0.3f, 0.7f, 0.7f,
            0.3f, 0.7f, 0.7f,
            0.3f, 0.5f, 0.9f,

            0.3f, 0.7f, 0.7f,
            0.3f, 0.7f, 0.7f,
            0.3f, 0.5f, 0.9f,

            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.3f, 0.5f, 0.9f,

            0.3f, 0.7f, 0.7f,
            0.3f, 0.7f, 0.7f,
            0.9f, 0.5f, 0.5f,

            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.9f, 0.5f, 0.5f,

            0.7f, 0.7f, 0.7f,
            0.7f, 0.7f, 0.7f,
            0.9f, 0.5f, 0.5f,

            0.3f, 0.7f, 0.7f,
            0.3f, 0.7f, 0.7f,
            0.9f, 0.5f, 0.5f
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    GLuint colorbuffer;
    glGenBuffers(1, &colorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);

    do {
        glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 1.0f, 50.0f);
        GLfloat X = sin(glfwGetTime()) * 5.0f;
        GLfloat Y = cos(glfwGetTime());
        GLfloat Z = cos(glfwGetTime()) * 3.0f;
        glm::mat4 View = glm::lookAt(glm::vec3(X, Y, Z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * Model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(programID);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        glEnableVertexAttribArray(vertexPosition_modelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                vertexPosition_modelspaceID,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *) nullptr
        );

        glEnableVertexAttribArray(vertexColorID);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                vertexColorID,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *) nullptr
        );

        glDrawArrays(GL_TRIANGLES, 0, 8 * 3);

        glDisableVertexAttribArray(vertexPosition_modelspaceID);
        glDisableVertexAttribArray(vertexColorID);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteBuffers(1, &colorbuffer);
    glDeleteProgram(programID);

    glfwTerminate();

    return 0;
}
