#version 400 core

uniform vec4 color;

in vec4 v;
in vec4 screen;

out vec4 fcolor;

const float scale = .2;

vec3 hsv2rgb(vec3 c) {
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

void main(){
    fcolor = color;
    fcolor.xyz *= smoothstep(scale, -scale, screen.z);
}
