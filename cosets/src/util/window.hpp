#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

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
