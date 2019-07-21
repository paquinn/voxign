#version 330 core

uniform vec3 resolution;
uniform vec3 volume;
uniform vec3 offset;
uniform float slice;

// [statics]

// [functions]

void main()
{
    // Put our coordinates on the [-0.5, 0.5] range
    vec2 uv = (gl_FragCoord.xy / resolution.xy) - 0.5;
    // Center of volume in xy plane
    vec2 xy = uv * volume.xy;
    // Center our slice on z axis
    float z = slice - (volume.z / 2.0);
    // Compute offset
    vec3 p = vec3(xy, z) - offset;

    // Check if we're inside an object
    float t = DE(p);
    float col = t > 0 ? 1.0 : 0.0;
    gl_FragColor = vec4(vec3(col), 1.0);
}


