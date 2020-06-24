#version 330 core

uniform vec3 volume;
uniform vec3 resolution;
uniform vec3 offset;
uniform float slice;

out vec4 color;

// [statics]

// [functions]

float S(vec3 p, float r) {
    return length(p) - r;
}

void main()
{
    // Calculate size of voxel
    vec3 voxel = volume / resolution;
    // Get integer index of voxel, (gl_FragCoord are not integer values)
    vec3 ind = vec3(floor(gl_FragCoord.xy), slice);
    // Calculate center point inside voxel
    vec3 center = voxel * (ind + vec3(0.5));
    // Center voxels on origin
    vec3 p = center - (volume / 2.0);

    // Check if we're inside an object
    float t = DE(p);
    float col = t <= 0.0 ? 1.0 : 0.0;

    color = vec4(vec3(col), 1.0);

}


