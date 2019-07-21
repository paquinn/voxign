from marcher.function import Primitive, Combinator, Operator
from marcher.vector import vec3, vec2


# --- Primitives --- #
@Primitive.register()
def Plane(p: vec3): """
    return p.y;
"""


@Primitive.register()
def Sphere(p: vec3, r: float) -> float: """
    return length(p) - r;
"""


@Primitive.register()
def Box(p: vec3, b: vec3): """
    vec3 d = abs(p) - b;
    return min(max(d.x, max(d.y, d.z)), 0.0) + length(max(d, 0.0));
"""


@Primitive.register()
def CylinderY(p: vec3, h: vec2): """
    vec2 d = abs(vec2(length(p.xz),p.y)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0)); 
"""


@Primitive.register()
def CylinderZ(p: vec3, h: vec2): """
    vec2 d = abs(vec2(length(p.xy),p.z)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
"""


@Primitive.register()
def CylinderX(p: vec3, h: vec2): """
    vec2 d = abs(vec2(length(p.yz),p.x)) - h;
    return min(max(d.x,d.y),0.0) + length(max(d,0.0));
"""


# --- Combinators --- #
@Combinator.register()
def Union(d1: float, d2: float): """
    return min(d1, d2);
"""


@Combinator.register()
def Intersect(d1: float, d2: float): """
    return max(d1, d2);
"""


@Combinator.register()
def Subtract(d1: float, d2: float): """
    return max(d1, -d2);
"""


@Operator.register()
def Translate(p: vec3, t: vec3): """
    return p - t; 
"""


@Operator.register()
def Repeat(p: vec3, n: vec3): """
    return mod(p, n) - n * 0.5; 
"""


@Operator.register()
def Mirror(p: vec3): """
    return abs(p);
"""