# basic.py
from marcher import *
from marcher.library import *


@Object.register()
def MyObject(self):
    self.res(Union, Box(vec3(1, 1, 1)))
    self.res(Intersect, Sphere(1.3))
    shape = vec2(0.5, 2)
    self.res(Subtract, CylinderX(shape))
    self.res(Subtract, CylinderY(shape))
    self.res(Subtract, CylinderZ(shape))

@Object.register()
def MyScene(self):
    mo = MyObject()
    for i in range(-1, 2):
        for j in range(-1, 2):
            pos = 2 * vec3(i, j, 0)
            self.res(Union, mo.at(pos))

@Object.register()
def Basic(self):
    # self.res(Union, Sphere(2.0).at(vec3(1, 2, 3)))
    bigR = 3.5
    s1 = 1.5
    s2 = 2.0
    s3 = 2.5
    self.res(Union, Sphere(bigR))
    self.res(Union, Sphere(s1).at(vec3(bigR + s1, 0, 0)))
    self.res(Union, Sphere(s2).at(vec3(0, bigR + s2, 0)))
    self.res(Union, Sphere(s3).at(vec3(0, 0, bigR + s3)))


# sdf = Object.MyObject
sdf = Object.Basic
