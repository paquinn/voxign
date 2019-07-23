#include "viewer.h"

Viewer::Viewer() :
        nanogui::Screen(nanogui::Vector2i(600, 600), "Voxign", false),
        mVoxels(nanogui::Vector3f(1.0, 1.0, 1.0), nanogui::Vector3i(100, 100, 100)),
        mCompiler()
{

}

void Viewer::setInputFile(const std::string &filename) {
    mCompiler.load(filename);
    mVoxels.setShader(mCompiler.reload("sliceFramework"));
    mRenderProgram.init("render", mCompiler.reload("renderFramework"));
}

void Viewer::setOutputFolder(const std::string &foldername) {
    mOutputFolder = foldername;
}

void Viewer::setBounds(Eigen::Array3i bounds, Eigen::Array3f voxels) {
    mVoxels.resizeBounds(bounds, voxels);
}

void Viewer::setVolume(Eigen::Array3f volume, Eigen::Array3f voxels) {
    mVoxels.resizeVolume(volume, voxels);
}

void Viewer::drawContents() {
    using namespace nanogui;

    if (mRenderProgram.ready()) {
        mRenderProgram.bind();
        mRenderProgram.setUniform("resolution", Vector2f{mFBSize[0], mFBSize[1]});
        mRenderProgram.setUniform("mouse", Vector2f{mMousePos[0], mMousePos[1]}, false);
        mRenderProgram.draw();
    }
}

Viewer::~Viewer() {
    mRenderProgram.free();
}