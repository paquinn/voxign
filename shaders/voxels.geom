#version 330

layout(points) in;
layout(triangle_strip, max_vertices = 12) out;

uniform mat4 mvp;
uniform vec3 voxelSize = vec3(0.25);
uniform vec3 light = normalize(vec3(0.4, -1.0, 0.8));

in vec3 gColor[];
in int gFaces[];

out vec3 fColor;

void EmitQuad(vec4 center, vec4 dy, vec4 dx, float brightness) {
    fColor = gColor[0] * brightness;
    gl_Position = center + (dx - dy);
    EmitVertex();

    fColor = gColor[0] * brightness;
    gl_Position = center + (-dx - dy);
    EmitVertex();

    fColor = gColor[0] * brightness;
    gl_Position = center + (dx + dy);
    EmitVertex();

    fColor = gColor[0] * brightness;
    gl_Position = center + (-dx + dy);
    EmitVertex();

    EndPrimitive();
}

bool Culled(vec4 norm) {
    return norm.z > 0;
}

void main() {

    vec4 center = gl_in[0].gl_Position;

    vec4 dx = mvp[0] / 2.0f * voxelSize.x;
    vec4 dy = mvp[1] / 2.0f * voxelSize.y;
    vec4 dz = mvp[2] / 2.0f * voxelSize.z;


    if ((gFaces[0] & 0x01) != 0 && !Culled(dx))
    EmitQuad(center + dx, dy, dz, max(dot(-light, vec3(1., 0., 0.)), 0.6));

    if ((gFaces[0] & 0x02) != 0 && !Culled(-dx))
    EmitQuad(center - dx, dz, dy, max(dot(-light, vec3(-1., 0., 0.)), 0.6));

    if ((gFaces[0] & 0x04) != 0 && !Culled(dy))
    EmitQuad(center + dy, dz, dx, max(dot(-light, vec3(0., 1., 0.)), 0.6));

    if ((gFaces[0] & 0x08) != 0 && !Culled(-dy))
    EmitQuad(center - dy, dx, dz, max(dot(-light, vec3(0., -1., 0.)), 0.6));

    if ((gFaces[0] & 0x10) != 0 && !Culled(dz))
    EmitQuad(center + dz, dx, dy, max(dot(-light, vec3(0., 0., 1.)), 0.6));

    if ((gFaces[0] & 0x20) != 0 && !Culled(-dz))
    EmitQuad(center - dz, dy, dx, max(dot(-light, vec3(0., 0., -1.)), 0.6));
}