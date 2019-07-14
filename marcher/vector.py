class vec:
    def __init__(self, *floats):
        self.floats = []
        # Able to build vecs out of other vecs as in glsl
        for f in floats:
            if type(f) is float or type(f) is int:
                # Make sure everything is a float here
                self.floats.append(float(f))
            elif type(f) is vec:
                self.floats.extend(f.floats)
            else:
                self.floats.append(f)
        self.dim = len(self.floats)

    def get_type(self):
        return 'vec' + str(self.dim)

    def __str__(self):
        return self.get_type() + '(' + ','.join([str(f) for f in self.floats]) + ')'

    def __mul__(self, scalar):
        # Make sure its a float
        scalar = float(scalar)
        return vec(*[scalar * f for f in self.floats])

    __rmul__ = __mul__


# For when you want to be sure of a vec dimension
def vec2(f1, f2):
    return vec(f1, f2)


def vec3(f1, f2, f3):
    return vec(f1, f2, f3)


def make_param(arg_type):
    if arg_type is float:
        return 'float'
    elif arg_type is vec3:
        return 'vec3'
    elif arg_type is vec2:
        return 'vec2'
