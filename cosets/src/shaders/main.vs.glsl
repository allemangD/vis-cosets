#version 440 core

uniform mat4 proj;
uniform mat4 view;

in vec4 pos;
out vec4 v;
out vec4 screen;

void main(){
  v = pos;

  vec4 vert = view * pos;

  /* stereographic projection */
  vert = vec4(vert.xyz / (1 - vert.w), 1);

  screen = gl_Position = proj * vert;
}
