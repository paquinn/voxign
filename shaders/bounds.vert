#version 330

uniform mat4 mvp;
in vec3 vPosition;
in vec3 vColor;

flat out lowp vec3 fColor;

void main() {
    gl_Position = mvp * vec4(vPosition, 1.0);
    fColor = vColor;
}