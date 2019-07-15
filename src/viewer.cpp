#include "viewer.h"

Viewer::Viewer() :
        nanogui::Screen(nanogui::Vector2i(600, 600), "Voxign", false),
        mVoxels(nanogui::Vector3f(0.5, 0.5, 0.1), nanogui::Vector3i(600, 600, 100))
{
    mVoxels.setSDF("");
}

void Viewer::drawContents() {
    using namespace nanogui;

    mVoxels.renderLayer(0);
}

Viewer::~Viewer() {
    mPreviewMarcherProgram.free();
    mPreviewSliceProgram.free();
    mPreviewVoxelProgram.free();
}