#version 330 core

uniform vec3 resolution;
uniform vec3 volume;
uniform vec3 offset;
uniform float slice;

// [statics]

// [functions]

float S(vec3 p, float r) {
    return length(p) - r;
}

void main()
{
    // Put our coordinates on the [-0.5, 0.5] range
    vec2 xy = (gl_FragCoord.xy / resolution.xy) - 0.5;
    float z = (slice / resolution.z) - 0.5;

    // Scale by our volume
    vec3 p = vec3(xy, z) * volume + offset;

    // Check if we're inside an object
    float t = DE(p);
    float col = t > 0 ? 1.0 : 0.0;

    gl_FragColor = vec4(vec3(col), 1.0);

}


