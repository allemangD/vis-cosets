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
   GLuint tri;

   GLuint verts;
   std::vector<glm::vec4> verts_data;

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

      verts_data = vertices<3>({}, Multiplicites<3>({
         {0, 1, 4},
         {1, 2, 3}
      }), {
         2.0f, .05f, 0.05f
      });

      glGenBuffers(1, &verts);
      glBindBuffer(GL_ARRAY_BUFFER, verts);
      glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * verts_data.size(), &verts_data[0], GL_STATIC_DRAW);


      glGenVertexArrays(1, &tri);
      glBindVertexArray(tri);
      glBindBuffer(GL_ARRAY_BUFFER, verts);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, nullptr);
      glBindBuffer(GL_ARRAY_BUFFER, 0);
   }

   void render() override {
      int w, h;
      getBounds(w, h);
      glViewport(0, 0, w, h);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      const glm::mat4 id = glm::mat4(1);
      const glm::vec3 ax = glm::vec3(0, 0, 1);
      double t = glfwGetTime();
      float angle = (float) t;

      float ar = (float) w / (float) h;

      const glm::mat4 proj = glm::ortho(-ar, ar, -1.f, 1.f, -1.f, 1.f);
      const glm::mat4 view = glm::rotate(id, angle, ax);
      const glm::mat4 mat = proj * view;

      glUseProgram(program);
      glUniformMatrix4fv(0, 1, false, glm::value_ptr(mat));
      glEnable(GL_DEPTH_TEST);
      glEnable(GL_POINT_SMOOTH);
      glPointSize(10.f);

      glBindVertexArray(tri);
      glDrawArrays(GL_POINTS, 0, verts_data.size());

      swapbuffers();
   }

   void deinit() override {
      glDeleteProgram(program);
      glDeleteVertexArrays(1, &tri);
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
