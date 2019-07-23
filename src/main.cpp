#include <pybind11/embed.h>
#include <Python.h>
#include <tuple>

#include "viewer.h"
#include "voxels.h"

using namespace std;
using namespace args;

namespace py = pybind11;

int parseArgs(const vector<string> &args) {

    ArgumentParser parser(
            "voxign - Voxel Sign\n"
                    "version " VOXIGN_VERSION "\n"
                    "Tool for inspecting and voxelizing SDFs\n",
            "Developed by Phillip Quinn <paquinn@cs.uw.edu>."
    );

    Flag versionFlag {parser, "VERSION", "Display the version of voxign", {"version"}};

    HelpFlag helpFlag {parser, "HELP", "Display help menu.", {'h', "help"}};

    ValueFlag<tuple3f> volumeFlag {parser, "VOLUME", "Volume for the voxelization. For verison number use --version.", {'v', "volume"}};
    ValueFlag<tuple3i> boundsFlag {parser, "BOUNDS", "Number of voxels in each dimension.", {'b', "bounds"}};
    ValueFlag<tuple3f> sizeFlag {parser, "SIZE", "Size of the voxels.", {'s', "size"}};

    Flag autoFlag {parser, "AUTO", "Auto mode will output voxelization without opening window. Must have INPUT and OUTPUT set.have INPUT and OUTPUT set.", {'a', "auto"}};

    Positional<string> inputFile(parser, "INPUT", "File to be loaded and viewed, can be changed later.");
    ValueFlag<string> outputFolder {parser, "OUTPUT", "Folder to save the voxelization layers.", {'o', "output"}};

    try {
        parser.Prog(args.front());
        parser.ParseArgs(begin(args) + 1, end(args));
    } catch (const Help &) {
        cout << "Help flag triggered" << endl;
        cout << parser;
        return 0;
    } catch (const ParseError &e) {
        cerr << e.what() << endl;
        cerr << parser;
        return -1;
    } catch (const ValidationError &e) {
        cerr << e.what() << endl;
        cerr << parser;
        return -2;
    }

    if (versionFlag) {
        cout << "version " << VOXIGN_VERSION << endl;
        return 0;
    }

    try {
        nanogui::init();
        py::scoped_interpreter guard{};
        {
            nanogui::ref<Viewer> screen = new Viewer();

            // TODO: This doesnt cover all cases and is not very robust
            if (sizeFlag)     {
                Eigen::Array3f voxelSize = tupleToArray3f(get(sizeFlag));
                if (boundsFlag) {
                    screen->setBounds(tupleToArray3i(get(boundsFlag)), voxelSize);
                } else if (volumeFlag) {
                    screen->setVolume(tupleToArray3f(get(volumeFlag)), voxelSize);
                } else {
                    throw ParseError{"Voxel size needs a boundry or volume"};
                }
            } else if (boundsFlag || volumeFlag) {
                throw ParseError{"Voxel size must be specified"};
            }
            if (inputFile)    { screen->setInputFile(get(inputFile)); }
            if (outputFolder) { screen->setOutputFolder(get(outputFolder)); }

            screen->drawAll();
            screen->setVisible(true);
            nanogui::mainloop(100);
        }
        nanogui::shutdown();
    } catch (const std::exception &e) {
        cerr << "Fatal error: " << e.what() << endl;
        return -1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    try {
        vector<string> args;
        for (int i = 0; i < argc; ++i) {
            args.emplace_back(argv[i]);
        }
        return parseArgs(args);
    } catch (const exception& e) {
        std::cerr << tfm::format("Uncaught exception: %s", e.what());
        return 1;
    }
}