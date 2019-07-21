#ifndef VOXIGN_COMPILER_H
#define VOXIGN_COMPILER_H

#include "common.h"
#include <pybind11/embed.h>
namespace py = pybind11;


class Compiler {
public:
    Compiler();

    bool load(const std::string &file);
    std::string reload(const std::string &framework);

private:
    py::module mCompiler;
    py::module mFramework;
    std::string mCurrentFile;

//    py::object mRenderFramework;
//    py::object mSDF;
//    bool successful;
//    std::string lastSuccessful;
//    std::string mError;
//    static bool initialized;

};
#endif //VOXIGN_COMPILER_H
