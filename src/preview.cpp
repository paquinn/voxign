#include "preview.h"

using namespace nanogui;


Preview::Preview(Widget *parent)
: GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f)) {
    mShaderVoxels.initFromFiles("voxels",
                                "shaders/voxels.vert",
                                "shaders/voxels.frag",
                                "shaders/voxels.geom");

//    MatrixXf faceIdxs(1, 6);
//    MatrixXu indices(3, 1);
    int length = 10;
    int size = length * length * length;
    MatrixXf positions(3, size);
    MatrixXf colors(3, size);
    MatrixXu faces(1, size);

    for (int z = 0; z < length; ++z) {
        for (int y = 0; y < length; ++y) {
            for (int x = 0; x < length; ++x) {
                int index = length * (length * z + y) + x;
                positions.col(index) << z - length/2, y - length/2, x - length/2;
                colors.col(index) << 0.6, 0.7, 0.8;
//                colors.col(index) << float(z) / length, float(y) / length, float(x) / length;
                faces.col(index) << 0xffffff;
            }
        }
    }

////    indices.col(0) << 0, 1, 2;
//
//    positions.col(0) << 0, 0, 0;
//    positions.col(1) << 0, 1, 0;
////    positions.col(1) << 0, 1, 1;
////    positions.col(2) << 1, 0, 0;
////    positions.col(3) << 0, 0, 0;
////    positions.col(4) << 0, 0, 0;
////    positions.col(5) << 0, 0, 0;
//
//    colors.col(0) << 0.1, 0.1, 0.9; // Blue
//    colors.col(1) << 0.9, 0.1, 0.9; // Blue
////    colors.col(1) << 0.9, 0.1, 0.1; // Red
////    colors.col(2) << 0.1, 0.9, 0.1; // Green
////    colors.col(3) << 0.5, 0.9, 0.8;
////    colors.col(4) << 0.5, 0.9, 0.8;
////    colors.col(5) << 0.5, 0.9, 0.8;
//
//    faces.col(0) << 0xffffff;
//    faces.col(1) << 0xffffff;

    mShaderVoxels.bind();
//    mShaderVoxels.uploadIndices(indices);
    mShaderVoxels.uploadAttrib("vPosition", positions);
    mShaderVoxels.uploadAttrib("vColor", colors);
    mShaderVoxels.uploadAttrib("vFaces", faces);
    mShaderVoxels.setUniform("bounds", Vector3f{length, length, length});

//    mShaderVoxels.uploadAttrib("vFaceIdx", faceIdxs);
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
//    mShaderVoxels.drawIndexed(GL_TRIANGLES, 0, 1);
//    mShaderVoxels.drawArray(GL_POINTS, 0, 1);
//    fTime = (float)glfwGetTime();
//    for (int z = 0; z < 100; ++z) {
////        for (int y = 0; y < 100; ++y) {
////            for (int x = 0; x < 100; ++x) {
//                int index = 100 * (100 * z + 0) + 0;
//                mShaderVoxels.drawArray(GL_POINTS, index, 100 * 100);
////            }
////        }
//    }
    mShaderVoxels.drawArray(GL_POINTS, 0, 10 * 10 * 10);
//    cout << fTime - (float)glfwGetTime()  << endl;
//    int i1 = 100 * (100 * 0 + 0) + 0;
//    int i2 = 100 * (100 * 5 + 5) + 5;
//    int i3 = 100 * (100 * 3 + 3) + 3;
    glDisable(GL_DEPTH_TEST);
}

Preview::~Preview() {
    mShaderVoxels.free();
}