# Base case for recursive compilation
class Var:
    def __init__(self, name):
        self.name = name

    def __call__(self, *_):
        return self

    def __str__(self):
        return self.name
