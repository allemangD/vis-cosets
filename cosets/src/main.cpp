#include "util/window.hpp"
#include "util/shader.hpp"

#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

class CosetsWindow : public Window {
   GLint program;
   GLuint tri;

   GLuint verts;
   static const unsigned VERTS_N = 8;

public:
   void init() override {
      auto vs = build_shader(GL_VERTEX_SHADER, "vertex",
         "#version 400\n"
         ""
         "uniform mat4 proj;"
         ""
         "in vec4 pos;"
         "out vec4 v;"
         ""
         "void main(){"
         "  v = pos;"
         ""
         "  /* stereographic projection */"
         "  vec4 vert = vec4(pos.xyz / (1 - pos.w), 1);"
         "  gl_Position = proj * vert;"
         "}");

      auto fs = build_shader(GL_FRAGMENT_SHADER, "fragment",
         "#version 400\n"
         ""
         "in vec4 v;"
         ""
         "vec3 hsv2rgb(vec3 c)\n"
         "{\n"
         "    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);\n"
         "    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);\n"
         "    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);\n"
         "}"
         ""
         "void main(){"
         "  gl_FragColor = vec4(hsv2rgb(vec3(v.w, 1, 1)), 0);"
         "}"
      );

      program = build_program("main", vs, fs);

      float verts_data[4 * VERTS_N]{
         +.5f, +.5f, 0.f, 0.f,
         +.5f, -.5f, 0.f, 0.f,
         -.5f, -.5f, 0.f, -.1f,
         -.5f, +.5f, 0.f, -.1f,

         +.5f, +.5f, 0.f, .1f,
         +.5f, -.5f, 0.f, .1f,
         -.5f, -.5f, 0.2f, .0f,
         -.5f, +.5f, 0.2f, .0f,
      };

      glGenBuffers(1, &verts);
      glBindBuffer(GL_ARRAY_BUFFER, verts);
      glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 4 * VERTS_N, verts_data, GL_STATIC_DRAW);


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
      glDrawArrays(GL_POINTS, 0, VERTS_N);

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