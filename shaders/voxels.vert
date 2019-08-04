#version 330

uniform mat4 mvp;

uniform vec3 voxelSize;
uniform vec3 volume;

attribute vec3 vPosition;
attribute vec3 vColor;
attribute float vFaces;

flat out lowp vec3 gColor;
flat out int gFaces;

void main() {
    // TODO: calculate proper positions on cpu
    gl_Position = mvp * vec4(vPosition * voxelSize - (volume - voxelSize) / 2.0, 1.0);
    // TODO: BUG: Currently a hack to fix issue with MatrixXi not being sent correctly
    gFaces = int(vFaces);
    gColor = vColor;
}