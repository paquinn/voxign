#version 330 core

uniform vec3 resolution;
uniform vec3 volume;
uniform vec3 offset;
uniform float slice;

#define DE(x) (MyObject((x),1e20))

float Union(float d1,float d2)
{
    return min(d1, d2);
}
vec3 Mirror(vec3 p)
{
    return abs(p);
}
vec3 Translate(vec3 p,vec3 t)
{
    return p - t;
}
float Sphere(vec3 p,float r)
{
    return length(p) - r;
}

float MyObject(vec3 p,float res)
{
    p=Mirror(p);
    res=Union(res,Sphere(Translate(p,vec3(0.5,1.0,0.5)),1.0));
    res=Union(res,Sphere(Translate(p,vec3(1.1,0.0,0.5)),1.0));
    return res;
}

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
//    gl_FragColor = vec4(vec3(col), 1.0);
    gl_FragColor = vec4(1.0, 0.2, 0.1, 1.0);
}


