#include "viewer.h"

Viewer::Viewer() :
        nanogui::Screen(nanogui::Vector2i(800, 600), "Voxign", false),
        mVoxels(nanogui::Vector3f(0.5, 0.5, 0.1), nanogui::Vector3i(100, 100, 100))
{

}

Viewer::~Viewer() {
    mPreviewMarcherProgram.free();
    mPreviewSliceProgram.free();
    mPreviewVoxelProgram.free();
}