#version 440 core

uniform mat4 proj;

in vec4 pos;
out vec4 v;
out vec4 screen;

void main(){
  v = pos;

  /* stereographic projection */
  vec4 vert = vec4(pos.xyz / (1 - pos.w), 1);
  screen = gl_Position = proj * vert;
}
