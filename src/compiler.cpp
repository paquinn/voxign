#include "compiler.h"

Compiler::Compiler() {
    mCompiler = py::module::import("marcher.compiler");
    mFramework = py::module::import("marcher.framework");
}

bool Compiler::load(const std::string &file) {
    mCurrentFile = file;
}

std::string Compiler::reload(const std::string &framework) {
    py::module sdf = py::module::import(mCurrentFile.c_str());
    // TODO: BUG: Temporary fix, when modules are reloaded they stay in the context
    sdf.reload();
    py::object root = sdf.attr("sdf");
    py::object shader = mCompiler.attr("compile")(root, mFramework.attr(framework.c_str()));
    std::string shader_string = shader.cast<std::string>();

    return shader_string;
}

