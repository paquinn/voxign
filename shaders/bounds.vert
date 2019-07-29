#version 330

uniform mat4 mvp;
attribute vec3 vPosition;

flat out lowp vec3 fColor;

void main() {
    gl_Position = mvp * vec4(vPosition, 1.0);
    fColor = vec3(1.0, 0.0, 0.0);
}