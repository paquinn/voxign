# basic.py
from marcher import *
from marcher.library import *

@Object.register()
def Basic(self):
    # self.res(Union, Sphere(2.0).at(vec3(1, 2, 3)))
    self.res(Union, Sphere(4.5))


sdf = Object.Basic

