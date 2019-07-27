#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 mvp;
uniform vec3 voxelSize = vec3(0.25);

in vec3 gColor[];
in int gFaceIdx[];

out vec3 fColor;

void EmitQuad(vec4 center, vec4 dy, vec4 dx) {
    fColor = gColor[0];
    gl_Position = center + (dx);
    EmitVertex();

    fColor = gColor[0];
    gl_Position = center;
    EmitVertex();

    fColor = gColor[0];
    gl_Position = center + (dx + dy);
    EmitVertex();

    fColor = gColor[0];
    gl_Position = center + (dy);
    EmitVertex();
}


void main() {
    const vec3 dxs[6] = vec3[6](
    vec3(0, 1, 0),
    vec3(0, 0, 1),
    vec3(0, 0, 1),
    vec3(1, 0, 0),
    vec3(1, 0, 0),
    vec3(0, 1, 0)
    );

    const vec3 dys[6] = vec3[6](
    vec3(0, 0, 1),
    vec3(0, 1, 0),
    vec3(1, 0, 0),
    vec3(0, 0, 1),
    vec3(0, 1, 0),
    vec3(1, 0, 0)
    );

    vec4 center = gl_in[0].gl_Position;
    EmitQuad(center,
    mvp * vec4(dxs[gFaceIdx[0]] * voxelSize, 0),
    mvp * vec4(dys[gFaceIdx[0]] * voxelSize, 0));
}