import os

from marcher import Function

def insert(into, outside, at):
    split = outside.split(at)
    assert len(split) == 2, "Can only split at one location not %r" % str(len(split) - 1)
    return split[0] + into + split[1]

def compile_statics(statics):
    s = ''
    for param, value in statics.items():
        s += '#define ' + param + ' ' + str(value) + '\n'
    return s

def compile_functions(root):
    stack = []
    root.toposort(set(), set(), stack)
    functions = ''
    for fn in reversed(stack):
        functions += str(Function.registry[fn]) + '\n'

    return functions

def compile(root, framework, **overrides):
    try:
        defaults = framework.get_statics()
        defaults = {**defaults, **overrides}

        statics = compile_statics(defaults)
        statics += '#define DE(x) ({}(x,1e20))'.format(root.name)

        functions = compile_functions(root)

        sdf_shader = framework.get_framework()
        sdf_shader = insert(statics, sdf_shader, '// [statics]')
        sdf_shader = insert(functions, sdf_shader, '// [functions]')

        return sdf_shader
    except Exception as e:
        return e




