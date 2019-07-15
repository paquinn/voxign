#include "viewer.h"
#include "voxels.h"

using namespace std;

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
        run_all();
    } else {
        try {
            nanogui::init();
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

