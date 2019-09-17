#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>

class Window {
protected:
   GLFWwindow *_window = nullptr;

public:
   virtual void init();

   virtual void update();

   virtual void render();

   virtual void deinit();

   void getBounds(int &width, int &height);

   void swapbuffers();

   void run();
};
