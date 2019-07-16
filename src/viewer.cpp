#include "viewer.h"

Viewer::Viewer() :
        nanogui::Screen(nanogui::Vector2i(600, 600), "Voxign", false),
        mVoxels(nanogui::Vector3f(1.0, 10.0, 0.1), nanogui::Vector3i(600, 600, 100))
{
//    mVoxels
    mVoxels.setSDF("");
    mVoxels.voxelizeLayer(0);
    mVoxels.saveLayer(0);
}

void Viewer::drawContents() {
    using namespace nanogui;

    mVoxels.voxelizeLayer(0);
    mVoxels.renderLayer(0);

}

Viewer::~Viewer() {
    mPreviewMarcherProgram.free();
    mPreviewSliceProgram.free();
    mPreviewVoxelProgram.free();
}