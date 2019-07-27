#include "preview.h"

using namespace nanogui;
Preview::Preview(Widget *parent)
: GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f)) {
    mShaderVoxels.initFromFiles("voxels",
                                "shaders/voxels.vert",
                                "shaders/voxels.frag",
                                "shaders/voxels.geom");

    MatrixXf faceIdxs(1, 6);
    MatrixXf positions(3, 6);
    MatrixXf colors(3, 6);

    faceIdxs.col(0) << 0;
    faceIdxs.col(1) << 1;
    faceIdxs.col(2) << 2;
    faceIdxs.col(3) << 3;
    faceIdxs.col(4) << 4;
    faceIdxs.col(5) << 5;

    positions.col(0) << 0, 0, 0;
    positions.col(1) << 0, 0, 0;
    positions.col(2) << 0, 0, 0;
    positions.col(3) << 0, 0, 0;
    positions.col(4) << 0, 0, 0;
    positions.col(5) << 0, 0, 0;

    colors.col(0) << 0.5, 0.9, 0.8;
    colors.col(1) << 0.5, 0.9, 0.8;
    colors.col(2) << 0.5, 0.9, 0.8;
    colors.col(3) << 0.5, 0.9, 0.8;
    colors.col(4) << 0.5, 0.9, 0.8;
    colors.col(5) << 0.5, 0.9, 0.8;

    mShaderVoxels.uploadAttrib("vPosition", positions);
    mShaderVoxels.uploadAttrib("vColor", colors);
    mShaderVoxels.uploadAttrib("vFaceIdx", faceIdxs);
}

void Preview::drawGL() {
    mShaderVoxels.bind();

    Matrix4f mvp;
    mvp.setIdentity();
    float fTime = (float)glfwGetTime();
    mvp.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0]*fTime, Vector3f::UnitX()) *
                                               Eigen::AngleAxisf(mRotation[1]*fTime,  Vector3f::UnitY()) *
                                               Eigen::AngleAxisf(mRotation[2]*fTime, Vector3f::UnitZ())) * 0.25f;

    mShaderVoxels.setUniform("mvp", mvp);


    glEnable(GL_DEPTH_TEST);
    mShaderVoxels.drawIndexed(GL_POINT, 0, 6);
    glDisable(GL_DEPTH_TEST);
}

Preview::~Preview() {
    mShaderVoxels.free();
}