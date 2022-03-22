//
// Created by Алина Орешина on 22/03/2022.
//

#include <cstdio>
#include <cstdlib>
#include <GL/glew.h>
#include <glfw3.h>
#include <glm/glm.hpp>
#include <common/shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

using namespace glm;

int main() {
    if (!glfwInit()) {
        fprintf(stderr, "Ошибка при инициализации GLFWn");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window;
    window = glfwCreateWindow(1024, 768, "Tutorial 01", nullptr, nullptr);
    if (window == nullptr) {
        fprintf(stderr,
                "Невозможно открыть окно GLFW. Если у вас Intel GPU, то он не поддерживает версию 3.3. Попробуйте версию уроков для OpenGL 2.1.n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glewExperimental = true;
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Невозможно инициализировать GLEWn");
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    static const GLfloat g_vertex_buffer_data[] = {
            -0.9f, -0.9f, 0.0f,
            0.2f, -0.9f, 0.0f,
            -0.2f, 0.9f, 0.0f,
            0.9f, -0.9f, 0.0f,
            -0.2f, -0.9f, 0.0f,
            0.4f, 0.4f, 0.0f,
    };

    GLuint vertexbuffer;
    glGenBuffers(1, &vertexbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);


    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    GLuint programID1 = LoadShaders("SimpleTransform.vertexshader", "SingleColor_1.fragmentshader");
    GLuint programID2 = LoadShaders("SimpleTransform.vertexshader", "SingleColor_2.fragmentshader");
    GLuint MatrixID = glGetUniformLocation(programID1, "MVP");

    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    do {
        glm::mat4 Projection = glm::perspective(45.0f, 1.0f, 1.0f, 50.0f);
        GLfloat X = sin(glfwGetTime()) * 5.0f;
        GLfloat Y = cos(glfwGetTime());
        GLfloat Z = cos(glfwGetTime()) * 3.0f;
        glm::mat4 View = glm::lookAt(glm::vec3(X, Y, Z), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * Model;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glUseProgram(programID1);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *) nullptr
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glDisableVertexAttribArray(0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glUseProgram(programID2);
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
        glVertexAttribPointer(
                0,
                3,
                GL_FLOAT,
                GL_FALSE,
                0,
                (void *) nullptr
        );
        glDrawArrays(GL_TRIANGLES, 3, 3);
        glDisableVertexAttribArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
             glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &vertexbuffer);
    glDeleteVertexArrays(2, &VertexArrayID);
    glDeleteProgram(programID1);
    glDeleteProgram(programID2);
    glfwTerminate();
}
