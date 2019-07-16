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
      }

      return s;
   }

public:
   void init() override {
      auto vs = build_shader(GL_VERTEX_SHADER, "vertex",
         "#version 400\n"
         ""
         "void main(){"
         "  int"
         "}");

      auto fs = build_shader(GL_FRAGMENT_SHADER, "fragment",
         "#version 400\n"
         ""
         "void main(){"
         "  int"
         "}"
      );

      program = glCreateProgram();
      glAttachShader(program, vs);
      glAttachShader(program, fs);
      glLinkProgram(program);
   }

   void render() override {

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