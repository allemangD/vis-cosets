#include "window.hpp"
#include "shader.hpp"
#include "mesh.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <iostream>
#include <iomanip>

using namespace std;

class CosetsWindow : public Window {
    GLint program;
    GLuint vert_vao, edge_vao, face_vao;

    Mesh *figure;

    GLint u_proj, u_view, u_color;

public:
    void init() override {
        std::cout
            << "Graphics Information:" << std::endl
            << "Vendor: " << glGetString(GL_VENDOR) << std::endl
            << "Renderer: " << glGetString(GL_RENDERER) << std::endl
            << "Version: " << glGetString(GL_VERSION) << std::endl
            << "Shading version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

        auto vs = build_shader_file(
            GL_VERTEX_SHADER,
            "vertex",
            "shaders/main.vs.glsl");

        auto fs = build_shader_file(
            GL_FRAGMENT_SHADER,
            "fragment",
            "shaders/main.fs.glsl");

        program = build_program("main", vs, fs);

        u_proj = glGetUniformLocation(program, "proj");
        u_view = glGetUniformLocation(program, "view");
        u_color = glGetUniformLocation(program, "color");

        figure = new Mesh(schlafli({5, 3, 3}));
        figure->gen_vertices(identity(figure->t_vert->mults, {10, .5, .5, .5}));

        glGenVertexArrays(1, &vert_vao);
        glBindVertexArray(vert_vao);
        glBindBuffer(GL_ARRAY_BUFFER, figure->vert.name);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glGenVertexArrays(1, &edge_vao);
        glBindVertexArray(edge_vao);
        glBindBuffer(GL_ARRAY_BUFFER, figure->vert.name);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure->edge.name);

        glGenVertexArrays(1, &face_vao);
        glBindVertexArray(face_vao);
        glBindBuffer(GL_ARRAY_BUFFER, figure->vert.name);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, figure->face.name);

        glBindVertexArray(0);
    }

    void render() override {
        int w, h;
        getBounds(w, h);
        glViewport(0, 0, w, h);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto id = glm::mat4(1);
        const auto ax_1 = glm::vec3(0, 0, 1);
        const auto ax_2 = glm::vec3(.5, 1, 0.2);
        const auto t = glfwGetTime();
        const auto angle = (float) t / 3;

        const float sc = 1.5f;
        const float ar = (float) w / (float) h;

        const float c1 = std::cos(angle * 3.0);
        const float s1 = std::sin(angle * 3.0);
        const float c2 = std::cos(angle * 0.2);
        const float s2 = std::sin(angle * 0.2);

        const glm::mat4 proj = glm::ortho(-ar * sc, ar * sc, -sc, sc, -10.f, 10.f);
        const glm::mat4 view = glm::rotate(id, angle, ax_1) * glm::rotate(id, angle, ax_2)
            * glm::mat4(
            c1, 0.0, s1, 0.0,
            0.0, c2, 0.0, s2,
            -s1, 0.0, c1, 0.0,
            0.0, -s2, 0.0, c2
        );

//        const float root_x = std::cos((t + 0) * M_PI / 2) / 2 + 0.5;
//        const float root_y = std::cos((t + 1) * M_PI / 2) / 2 + 0.5;
//        const float root_z = std::cos((t + 2) * M_PI / 2) / 2 + 0.5;
//        const float root_w = std::cos((t + 3) * M_PI / 2) / 2 + 0.5;
//
//        const glm::vec4 root = identity(figure->mults, {root_x, root_y, root_z, root_w});
//        figure->gen_vertices(root);

        glUseProgram(program);
        glUniformMatrix4fv(u_proj, 1, false, glm::value_ptr(proj));
        glUniformMatrix4fv(u_view, 1, false, glm::value_ptr(view));

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_POINT_SMOOTH);
        glEnable(GL_BLEND);
        glEnable(GL_CULL_FACE);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glPointSize(10.0f);
        glLineWidth(5.0f);

        glBindVertexArray(vert_vao);
        glUniform4f(u_color, 1, 0, 0, 1);
        glDrawArrays(GL_POINTS, 0, figure->vert.size);

        glBindVertexArray(edge_vao);
        glUniform4f(u_color, 1, 0, 0, 1);
        glDrawElements(GL_LINES, figure->edge.size, GL_UNSIGNED_INT, 0);

//      glBindVertexArray(face_vao);
//      glCullFace(GL_NONE);
//      glUniform4f(u_color, 1, 1, 1, 1);
//      glDrawElements(GL_TRIANGLES, figure->face.size, GL_UNSIGNED_INT, 0);

//      glCullFace(GL_FRONT);
//      glUniform4f(u_color, .3, .3, 1, 1);
//      glDrawElements(GL_TRIANGLES, figure->face.size, GL_UNSIGNED_INT, 0);

        swapbuffers();
    }

    void deinit() override {
        glDeleteProgram(program);
        glDeleteVertexArrays(1, &vert_vao);
    }
};

int main(int argc, char *argv[]) {
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return EXIT_FAILURE;
    }

    Window *window = new CosetsWindow();
    window->run();

    glfwTerminate();
    return EXIT_SUCCESS;
}
