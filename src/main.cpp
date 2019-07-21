#include <pybind11/embed.h>
#include <Python.h>

#include "viewer.h"
#include "voxels.h"

using namespace std;
namespace py = pybind11;

void run_all() {
    nanogui::init();
    test_clear();
    test_finishedAll();
    test_resizeVolume();
    nanogui::shutdown();
}

int main(int argc, char* argv[]) {
    bool run_tests = false;

    if (run_tests) {
        py::scoped_interpreter guard{};

        py::module compiler = py::module::import("marcher.compiler");
        py::module framework = py::module::import("marcher.framework");
        py::module sdf = py::module::import("basic");
        py::object root = sdf.attr("sdf");

        py::print(compiler.attr("compile")(root, framework.attr("renderFramework")));

    } else {
        try {
            nanogui::init();
            py::scoped_interpreter guard{};
            {
                nanogui::ref<Viewer> screen = new Viewer();
                screen->drawAll();
                screen->setVisible(true);
                nanogui::mainloop(100);
            }
            nanogui::shutdown();
        } catch (const std::exception &e) {
            cerr << "Fatal error: " << e.what() << endl;
            return -1;
        }
    }

    return 0;
}

