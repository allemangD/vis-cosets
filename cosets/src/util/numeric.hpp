#pragma once

#include <glm/glm.hpp>

glm::vec4 round(glm::vec4 f, int prec) {
   auto dec = (float) pow(10, prec);
   auto res = glm::trunc(f * dec + 0.5f) / dec;
   return res;
}

float dot(int n, glm::vec4 a, glm::vec4 b) {
   float sum = 0;
   for (int i = 0; i < n; ++i) {
      sum += a[i] * b[i];
   }
   return sum;
}
