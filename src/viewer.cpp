#include "viewer.h"

Viewer::Viewer() :
        nanogui::Screen(nanogui::Vector2i(600, 600), "Voxign", false),
        mVoxels(nanogui::Vector3f(1.0, 1.0, 1.0), nanogui::Vector3i(100, 100, 100)),
        mCompiler()
{
//    mCompiler.load("basic");
//    mVoxels.setShader(mCompiler.reload("sliceFramework"));
//    mRenderProgram.init("render", mCompiler.reload("renderFramework"));
}

void Viewer::setInputFile(const std::string &filename) {
    mCompiler.load(filename);
    mVoxels.setShader(mCompiler.reload("sliceFramework"));
    mRenderProgram.init("render", mCompiler.reload("renderFramework"));
}

void Viewer::setOutputFolder(const std::string &foldername) {
    mOutputFolder = foldername;
}

void Viewer::setVoxelSize(Eigen::Array3f size) {
    mVoxels.resizeVoxels(size);
}

void Viewer::setBounds(Eigen::Array3i bounds) {
    mVoxels.resizeBounds(bounds);
}

void Viewer::setVolume(Eigen::Array3f volume) {
    mVoxels.resizeVolume(volume);
}

void Viewer::drawContents() {
    using namespace nanogui;

    mRenderProgram.bind();
    mRenderProgram.setUniform("resolution", Vector2f{mFBSize[0], mFBSize[1]});
    mRenderProgram.setUniform("mouse", Vector2f{mMousePos[0], mMousePos[1]}, false);
    mRenderProgram.draw();
}

Viewer::~Viewer() {
    mRenderProgram.free();
}