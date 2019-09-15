#include "util/window.hpp"
#include "util/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "util/mesh.hpp"

using namespace std;

class CosetsWindow : public Window {
   GLint program;
   GLuint points_vao, edges_vao;

   GLuint verts_buf;
   std::vector<glm::vec4> verts_data;

   GLuint edges_buf;
   std::vector<int> edge_data;

public:
   void init() override {
      auto vs = build_shader_file(
         GL_VERTEX_SHADER,
         "vertex",
         "shaders/main.vs.glsl");

      auto fs = build_shader_file(
         GL_FRAGMENT_SHADER,
         "fragment",
         "shaders/main.fs.glsl");

      program = build_program("main", vs, fs);

      const Mults &mults = schlafli<4>({3, 4, 3});
      verts_data = vertices<4>(mults, {10, 1, 1, 1});
      edge_data = edges<4>(mults);

      glGenBuffers(1, &verts_buf);
      glBindBuffer(GL_ARRAY_BUFFER, verts_buf);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * verts_data.size(), &verts_data[0], GL_STATIC_DRAW);

      glGenBuffers(1, &edges_buf);
      glBindBuffer(GL_ARRAY_BUFFER, edges_buf);
      glBufferData(GL_ARRAY_BUFFER, sizeof(int) * edge_data.size(), &edge_data[0], GL_STATIC_DRAW);

      glGenVertexArrays(1, &points_vao);
      glBindVertexArray(points_vao);
      glBindBuffer(GL_ARRAY_BUFFER, verts_buf);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
      glBindBuffer(GL_ARRAY_BUFFER, 0);

      glGenVertexArrays(1, &edges_vao);
      glBindVertexArray(edges_vao);
      glBindBuffer(GL_ARRAY_BUFFER, verts_buf);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, edges_buf);

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
      const auto angle = (float) t / 2;

      const float sc = 1.5f;
      const float ar = (float) w / (float) h;

      const glm::mat4 proj = glm::ortho(-ar * sc, ar * sc, -sc, sc, -10.f, 10.f);
      const glm::mat4 view = glm::rotate(id, angle, ax_1) * glm::rotate(id, angle, ax_2);

      glUseProgram(program);
      glUniformMatrix4fv(1, 1, false, glm::value_ptr(proj));
      glUniformMatrix4fv(2, 1, false, glm::value_ptr(view));
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_POINT_SMOOTH);
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glPointSize(10.f);

      glBindVertexArray(points_vao);
      glUniform4f(0, 1, 1, 1, 1);
      glDrawArrays(GL_POINTS, 0, verts_data.size());

      glBindVertexArray(edges_vao);
      glUniform4f(0, 1, 1, 0, 1);
      glDrawElements(GL_LINES, edge_data.size(), GL_UNSIGNED_INT, 0);

      swapbuffers();
   }

   void deinit() override {
      glDeleteProgram(program);
      glDeleteVertexArrays(1, &points_vao);
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
