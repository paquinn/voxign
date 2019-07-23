import os

from marcher import vec3


class Framework:
    def __init__(self, file, statics):
        self.file = file
        self.statics = statics

    def read_file(self, file):
        os.path.join(os.path.dirname(__file__), file)
        if os.path.exists(file):
            framework = open(file).read()
            return framework
        else:
            print("File not found {}".format(file))
            raise FileNotFoundError

    def get_framework(self):
        return self.read_file(self.file)

    def get_statics(self):
        return self.statics

renderFramework = Framework("shaders/render.glsl",
                            {"MAX_STEPS": 100,
                             "MAX_DISTANCE": 100.0,
                             "MIN_DISTANCE": 0.001,
                             "AA": 2,
                             "BACKGROUND": vec3(0.7, 0.9, 1.0),
                             "MATERIAL": vec3(0.5, 0.6, 0.6),
                             "RO": vec3(1, 1, 1),
                             "TA": vec3(0, 0, 0),
                             "LIGHT_POS": vec3(1, 4, 1),
                             "LOOK": 1
                             })

sliceFramework = Framework("shaders/slice.glsl", {})