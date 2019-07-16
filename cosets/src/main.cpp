#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<string>
#include<stdio.h>

using namespace std;

class Window {
protected:
   GLFWwindow *_window = nullptr;

public:
   virtual void init() {}

   virtual void update() {}

   virtual void render() {}

   virtual void deinit() {}

   void getBounds(int &width, int &height) {
      glfwGetFramebufferSize(_window, &width, &height);
   }

   void swapbuffers() {
      glfwSwapBuffers(_window);
   }

   void run() {
      int _gl_major = 4, _gl_minor = 0;
      glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _gl_major);
      glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _gl_minor);
      glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


      _window = glfwCreateWindow(640, 480, "GLFW App", nullptr, nullptr);
      if (!_window) {
         fprintf(stderr, "Failed to create window;");
         glfwTerminate();
         exit(EXIT_FAILURE);
      }

      glfwMakeContextCurrent(_window);
      gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
      glfwSwapInterval(0);

      init();

      while (!glfwWindowShouldClose(_window)) {
         update();
         render();

         glfwPollEvents();
      }

      deinit();

      glfwDestroyWindow(_window);
   }
};

class CosetsWindow : public Window {
   GLint program;
   GLuint vao;

   GLint build_shader(GLenum type, const string &name, const string &src) {
      const char *c_str = src.c_str();

      auto s = glCreateShader(type);
      glShaderSource(s, 1, &c_str, nullptr);
      glCompileShader(s);

      GLint stat;
      glGetShaderiv(s, GL_COMPILE_STATUS, &stat);
      if (!stat) {
         GLint log_len;
         glGetShaderiv(s, GL_INFO_LOG_LENGTH, &log_len);
         char log[log_len];
         glGetShaderInfoLog(s, log_len, nullptr, log);
         fprintf(stderr, "SHADER LOG (%s):\n%s", name.c_str(), log);

         glDeleteShader(s);
         return 0;
      }

      return s;
   }

   GLint build_program(const string &name, GLint vs, GLint fs) {
      GLint p = glCreateProgram();
      glAttachShader(p, vs);
      glAttachShader(p, fs);
      glLinkProgram(p);

      GLint stat;
      glGetProgramiv(p, GL_LINK_STATUS, &stat);
      if (!stat) {
         GLint log_len;
         glGetShaderiv(p, GL_INFO_LOG_LENGTH, &log_len);
         char log[log_len];
         glGetProgramInfoLog(p, log_len, nullptr, log);
         fprintf(stderr, "SHADER LOG (%s):\n%s", name.c_str(), log);

         glDeleteProgram(p);
         return 0;
      }

      return p;
   }

public:
   void init() override {
      auto vs = build_shader(GL_VERTEX_SHADER, "vertex",
         "#version 400\n"
         "void main(){"
         "  float x = gl_VertexID % 2;"
         "  float y = gl_VertexID / 2;"
         "  gl_Position = vec4(x, y, 0, 1);"
         "}");

      auto fs = build_shader(GL_FRAGMENT_SHADER, "fragment",
         "#version 400\n"
         "void main(){"
         "  gl_FragColor = vec4(0, 1, 1, 0);"
         "}"
      );

      program = build_program("main", vs, fs);

      glGenVertexArrays(1, &vao);
   }

   void render() override {
      int w, h;
      getBounds(w, h);
      glViewport(0, 0, w, h);

      glBindVertexArray(vao);
      glUseProgram(program);
      glDrawArrays(GL_TRIANGLES, 0, 3);

      swapbuffers();
   }

   void deinit() override {
      glDeleteProgram(program);
      glDeleteVertexArrays(1, &vao);
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