#include "window.hpp"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>

void Window::init() {}

void Window::update() {}

void Window::render() {}

void Window::deinit() {}

void Window::getBounds(int &width, int &height) {
   glfwGetFramebufferSize(_window, &width, &height);
}

void Window::swapbuffers() {
   glfwSwapBuffers(_window);
}

void Window::run() {
   int _gl_major = 4, _gl_minor = 0;
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, _gl_major);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, _gl_minor);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_ALPHA_BITS, 8);


   _window = glfwCreateWindow(1920, 1080, "GLFW App", nullptr, nullptr);
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
