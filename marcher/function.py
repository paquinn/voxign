import inspect

from marcher.variable import Var
from marcher.vector import make_param, vec3


class Function:
    registry = {}
    partials = {}

    class Call:
        def __init__(self, name, args):
            self.name = name
            self.args = list(args)

        def get_args(self):
            return self.args

        def get_usage(self):
            usage = set([self.name])
            for arg in self.get_args():
                if isinstance(arg, Function.Call):
                    usage |= arg.get_usage()
            return usage

        def __str__(self):
            args_list = ','.join([str(arg) for arg in self.get_args()])
            return self.name + '(' + args_list + ')'

    def _call(self, *args):
        return self.__class__.Call(self.name, args)

    def _partial_call(self, *second):
        def partial(*first):
            return self._call(*(first + second))

        return partial

    @staticmethod
    def _default_return():
        return None

    @classmethod
    def register(cls, *dependencies):
        def decorator(fn):
            fun = cls(fn, dependencies)
            setattr(cls, fun.name, fun)
            cls.registry[fun.name] = fun
            cls.partials[fun._call] = fun._partial_call
            return fun._call

        return decorator

    def __init__(self, fn, dependencies):
        self.name = fn.__name__
        # TODO: BUG: Temporary fix to allow reloading
        # assert self.name not in self.registry, "Name %r has already been used" % self.name
        self.params = fn.__annotations__.copy()
        default = self._default_return()
        self.return_type = self.params.pop('return', default)
        assert self.return_type, "%r has no return type specified and no default type" % self.name
        assert not default or self.return_type == default, \
            "r%'s specified return type does not match default" % self.name

        self.fn = fn
        self.dependencies = set(dependencies)

    def get_body(self):
        split = inspect.getsource(self.fn).split('"""')
        assert (len(split) == 3), "%r has no body" % self.name
        return split[1].strip()

    def get_dependencies(self):
        return self.dependencies

    def toposort(self, visited, cycle, stack):
        visited.add(self.name)
        cycle.add(self.name)
        for dep in self.get_dependencies():
            if dep not in visited:
                self.registry[dep].toposort(visited, cycle, stack)
            elif dep in cycle:
                assert False, "Cycle detected, recursion is not supported"
        stack.insert(0, self.name)
        cycle.remove(self.name)

    def __str__(self):
        fun = make_param(self.return_type) + ' ' + self.name
        fun += '(' + ','.join([make_param(arg_type) + ' ' + arg for arg, arg_type in self.params.items()]) + ')\n'
        fun += '{\n'
        fun += self.get_body()
        fun += '\n}'
        return fun


class Primitive(Function):
    class Call(Function.Call):
        def __init__(self, name, args, location, f=None):
            super().__init__(name, args)
            self.location = location
            self.f = f

        def __call__(self, f):
            apply = f if not self.f else self.f(f)
            return self.__class__(self.name, self.args.copy(), self.location, apply)

        def at(self, location):
            return self.__class__(self.name, self.args.copy(), location, self.f)

        def f(self, f):
            return self.__class__(self.name, self.args.copy(), self.location, f(self.f))

        def wrap_location(self):
            if self.location:
                from marcher.library import Translate
                return Translate(self.location, f=self.f)
                # return self.f
            else:
                return self.f

        def get_args(self):
            assert self.f, "Compiling a partial"
            return [self.wrap_location()] + self.args

    @staticmethod
    def _default_return():
        return float

    def _call(self, *args, at=None, f=None):
        return self.__class__.Call(self.name, args, at, f)


class Combinator(Function):
    class Call(Function.Call):

        def at(self, location: vec3):
            return self.__class__(self.name,
                                  [arg.at(location) if isinstance(arg, Function.Call) else arg for arg in self.args])

        def f(self, f):
            return self.__class__(self.name, [arg.f(f) if isinstance(arg, Function.Call) else arg for arg in self.args])

        def __call__(self, f):
            return self.__class__(self.name, [arg(f) if isinstance(arg, Function.Call) else arg for arg in self.args])

    @staticmethod
    def _default_return():
        return float


class Operator(Function):
    class Call(Function.Call):

        def __init__(self, name, args, f=None):
            super().__init__(name, args)
            self.f = f

        def __call__(self, f):
            apply = f if not self.f else self.f(f)
            return self.__class__(self.name, self.args.copy(), apply)

        # TODO Wrong way around
        def __mul__(self, other):
            return self.__class__(other.name, other.args.copy(), self)

        # __rmul__ = __mul__

        def get_args(self):
            assert self.f, "Compiling a partial"
            return [self.f] + self.args

    @staticmethod
    def _default_return():
        return vec3

    def _call(self, *args, f=None):
        return self.__class__.Call(self.name, args, f)


class Object(Primitive):
    class Call(Primitive.Call):

        def get_args(self):
            assert self.f, "Compiling a partial"
            return self.args + [self.wrap_location(), Var('res')]

    def __init__(self, fn, dependencies):
        super().__init__(fn, dependencies)
        # Need to make sure the p and res args come at the beginning
        # tmp = self.params
        # self.params = OrderedDict()
        self.params['p'] = vec3
        self.params['res'] = float
        # for arg, arg_type in tmp.items():
        #     self.params[arg] = arg_type
        self.lines = []
        # self.usages = set()

    def _call(self, *args, at=None, f=None):
        return super()._call(*args, at=at, f=f)

    # def res(self, fn, *args):
    #     self.lines.append(self.partials[fn](args)(Var('res'))(Var('p')))

    def res(self, fn, *args):
        line = (Var('res'), fn(*((Var('res'),) + args))(Var('p')))
        self.lines.append(line)
        self.dependencies |= line[1].get_usage()

    def p(self, fn, *args):
        if isinstance(fn, Operator.Call):
            line = (Var('p'), fn(Var('p')))
        else:
            line = (Var('p'), fn(*args)(Var('p')))
        self.lines.append(line)
        self.dependencies |= line[1].get_usage()

    # Lazy evaluation of function for body and dependencies
    def evaluate(self):
        if not self.lines:
            dummy_args = (len(self.fn.__annotations__)) * [None]
            self.fn(*([self] + dummy_args))

    def gen_body(self):
        body = ''
        for var, value in self.lines:
            body += str(var) + '=' + str(value) + ';\n'
        body += 'return ' + str(Var('res')) + ';'
        return body

    def get_body(self):
        self.evaluate()
        return self.gen_body()

    def get_dependencies(self):
        self.evaluate()
        return self.dependencies
