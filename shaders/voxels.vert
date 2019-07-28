#version 330

uniform mat4 mvp;
uniform vec3 voxelSize;

in vec3 vPosition;
in vec3 vColor;
in int vFaces;

out vec3 gColor;
out int gFaces;

void main() {
    gl_Position = mvp * vec4(vPosition * voxelSize, 1.0);
    gFaces = vFaces;
    gColor = vColor;
}