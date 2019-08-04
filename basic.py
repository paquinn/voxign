# basic.py
from marcher import *
from marcher.library import *

@Object.register()
def Basic(self):
    # self.res(Union, Sphere(2.0).at(vec3(1, 2, 3)))
    bigR = 4.5
    s1 = 1.5
    s2 = 2.0
    s3 = 2.5
    self.res(Union, Sphere(bigR))
    self.res(Union, Sphere(s1).at(vec3(bigR + s1, 0, 0)))
    self.res(Union, Sphere(s2).at(vec3(0, bigR + s2, 0)))
    self.res(Union, Sphere(s3).at(vec3(0, 0, bigR + s3)))


sdf = Object.Basic

