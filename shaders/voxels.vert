#version 330

uniform mat4 mvp;
uniform vec3 voxelSize = vec3(0.25);

//uniform vec3 vOffset = vec3(0.0);

in vec3 vPosition;
in vec3 vColor;
in int vFaceIdx;

out vec3 gColor;
out int gFaceIdx;

void main() {
    gl_Position = mvp * vec4(vPosition * voxelSize, 1.0);
    gFaceIdx = vFaceIdx;
    gColor = vColor;
}