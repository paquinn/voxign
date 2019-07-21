#include "viewer.h"

Viewer::Viewer() :
        nanogui::Screen(nanogui::Vector2i(600, 600), "Voxign", false),
        mVoxels(nanogui::Vector3f(1.0, 1.0, 1.0), nanogui::Vector3i(100, 100, 100)),
        mCompiler()
{
    mCompiler.load("basic");
    mVoxels.setShader(mCompiler.reload("sliceFramework"));
    auto file_to_string = [](const std::string &filename) -> std::string {
        if (filename.empty())
            return "";
        std::ifstream t(filename);
        return std::string((std::istreambuf_iterator<char>(t)),
                           std::istreambuf_iterator<char>());
    };
    std::string shader =  mCompiler.reload("renderFramework");
    cout << shader << endl;
//    mRenderProgram.init("render", file_to_string("shaders/pass.glsl"), file_to_string("shaders/test.glsl"));
    mRenderProgram.init("render", file_to_string("shaders/pass.glsl"), shader);

    nanogui::MatrixXu indices(3, 2);
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 1, 3;
    MatrixXf positions(3, 4);
    positions.col(0) << -1, -1, 0;
    positions.col(1) << 1, -1, 0;
    positions.col(2) << -1, 1, 0;
    positions.col(3) << 1, 1, 0;
    // bind the shader and upload vertex positions and indices
    mRenderProgram.bind();
    mRenderProgram.uploadIndices(indices);
    mRenderProgram.uploadAttrib("position", positions);
}

void Viewer::setFile(const std::string &filename) {
    mCompiler.load(filename);
}

void Viewer::setVoxelSize(Eigen::Vector3f size) {
    mVoxels.resizeVoxels(size);
}

void Viewer::setBounds(Eigen::Vector3i bounds) {
    mVoxels.resizeBounds(bounds);
}

void Viewer::setVolume(Eigen::Vector3f volume) {
    mVoxels.resizeVolume(volume);
}

void Viewer::drawContents() {
    using namespace nanogui;

    mRenderProgram.bind();
    mRenderProgram.setUniform("resolution", Vector2f{mFBSize[0], mFBSize[1]}, false);
    mRenderProgram.setUniform("mouse", Vector2f{mMousePos[0], mMousePos[1]}, false);
    glEnable(GL_DEPTH_TEST);
    mRenderProgram.drawIndexed(GL_TRIANGLES, 0, 2);
    glDisable(GL_DEPTH_TEST);

//    mVoxels.voxelizeLayer(0);
//    mVoxels.renderLayer(0);

}

Viewer::~Viewer() {
//    mPreviewMarcherProgram.free();
//    mPreviewSliceProgram.free();
//    mPreviewVoxelProgram.free();
    mRenderProgram.free();
}