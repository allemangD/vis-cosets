#version 400

uniform mat4 proj;

in vec4 pos;
out vec4 v;

void main(){
  v = pos;

  /* stereographic projection */
  vec4 vert = vec4(pos.xyz / (1 - pos.w), 1);
  gl_Position = proj * vert;
}
