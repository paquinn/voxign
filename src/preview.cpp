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
//    int length = 10;
//    int size = length * length * length;
//    MatrixXf positions(3, size);
//    MatrixXf colors(3, size);
//    MatrixXu faces(1, size);
//
//    for (int z = 0; z < length; ++z) {
//        for (int y = 0; y < length; ++y) {
//            for (int x = 0; x < length; ++x) {
//                int index = length * (length * z + y) + x;
//                positions.col(index) << z - length/2, y - length/2, x - length/2;
//                colors.col(index) << 0.6, 0.7, 0.8;
////                colors.col(index) << float(z) / length, float(y) / length, float(x) / length;
//                faces.col(index) << 0xffffff;
//            }
//        }
//    }

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

//    mShaderVoxels.bind();
////    mShaderVoxels.uploadIndices(indices);
//    mShaderVoxels.uploadAttrib("vPosition", positions);
//    mShaderVoxels.uploadAttrib("vColor", colors);
//    mShaderVoxels.uploadAttrib("vFaces", faces);
//    mShaderVoxels.setUniform("bounds", Vector3f{length, length, length});
//    mShaderVoxels.uploadAttrib("vFaceIdx", faceIdxs);
}

void Preview::setVoxels(Voxels *pVoxels) {
    // TODO: Might not be needed
//    if (mShaderVoxels.hasAttrib("vPosition")) {
//        mShaderVoxels.freeAttrib("vPosition");
//        mShaderVoxels.freeAttrib("vColor");
//        mShaderVoxels.freeAttrib("vFaces");
//    }

    mVoxels = pVoxels;
    setViewInterval(0, mVoxels->layerCount());
    setLayer(0.0);

    int voxelCount = mVoxels->layerCount() * mVoxels->layerSize().prod();
    MatrixXf positions(3, voxelCount);
    MatrixXf colors(3, voxelCount);
    MatrixXu faces(1, voxelCount);
    int solidCount = 0;
    int layers = mVoxels->layerCount();
    Array2i size = mVoxels->layerSize();
    int width = size.coeff(0);
    int height = size.coeff(1);

    for (int z = 0; z < layers; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (isSolid(mVoxels->index(x, y, z))) {
//                    int index = width * (height * z + y) + x;
//                    positions.col(solidCount) << x, y, z;
                    positions.col(solidCount) << z - layers/2, y - height/2, x - width/2;
                    colors.col(solidCount) << 1.0, 1.0, 1.0;
                    faces.col(solidCount) << 0xffffff;
//                    tfm::printfln("%s, %s, %s", x, y, z);
                    solidCount++;
                }
//                int index = length * (length * z + y) + x;
//                positions.col(index) << z - length/2, y - length/2, x - length/2;
//                colors.col(index) << 0.6, 0.7, 0.8;
////                colors.col(index) << float(z) / length, float(y) / length, float(x) / length;
//                faces.col(index) << 0xffffff;
            }
        }
    }

    mShaderVoxels.bind();
    mShaderVoxels.uploadAttrib("vPosition", positions);
    mShaderVoxels.uploadAttrib("vColor", colors);
    mShaderVoxels.uploadAttrib("vFaces", faces);
    mSolidCount = solidCount;

    tfm::printfln("Solid count: %s/%s", solidCount, voxelCount);
    mReady = true;
}

void Preview::setLayer(float layer) {
    mLayer = layer;
}

void Preview::setViewInterval(int start, int end) {
    mStart = start;
    mEnd = end;
}

void Preview::drawGL() {
    if (mReady) {
        mShaderVoxels.bind();

        Matrix4f mvp;
        mvp.setIdentity();
        float fTime = (float) glfwGetTime();
        mvp.topLeftCorner<3, 3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0] * fTime, Vector3f::UnitX()) *
                                                    Eigen::AngleAxisf(mRotation[1] * fTime, Vector3f::UnitY()) *
                                                    Eigen::AngleAxisf(mRotation[2] * fTime, Vector3f::UnitZ())) * 0.25f;

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
//        cout << mSolidCount << endl;
        mShaderVoxels.drawArray(GL_POINTS, 0, mSolidCount);
        glDisable(GL_DEPTH_TEST);
    }
}

bool Preview::isSolid(RGB voxel) {
    return voxel.isApprox(mEmpty);
}

void Preview::printVoxels() {
    int layers = mVoxels->layerCount();
    Array2i size = mVoxels->layerSize();
    int width = size.coeff(0);
    int height = size.coeff(1);
    
    for (int z = 0; z < layers; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (mVoxels->index(x, y, z).r() < 0.5) {
                    cout << "X";
                } else {
                    cout << "O";
                }
            }
            cout << endl;
        }
        cout << endl;
    }
}

Preview::~Preview() {
    mShaderVoxels.free();
}