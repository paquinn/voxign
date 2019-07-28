#include "preview.h"

using namespace nanogui;


Preview::Preview(Widget *parent)
: GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f)) {
    mShaderVoxels.initFromFiles("voxels",
                                "shaders/voxels.vert",
                                "shaders/voxels.frag",
                                "shaders/voxels.geom");
}

void Preview::setVoxels(Voxels *pVoxels) {
    // TODO: Might not be needed
    if (mShaderVoxels.hasAttrib("vPosition")) {
        mShaderVoxels.freeAttrib("vPosition");
        mShaderVoxels.freeAttrib("vColor");
        mShaderVoxels.freeAttrib("vFaces");
    }

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
                    positions.col(solidCount) << z - layers/2, y - height/2, x - width/2;
                    colors.col(solidCount) << 1.0, 1.0, 1.0;
                    faces.col(solidCount) << 0xffffff;
                    solidCount++;
                }
            }
        }
    }

    mShaderVoxels.bind();
    mShaderVoxels.uploadAttrib("vPosition", positions);
    mShaderVoxels.uploadAttrib("vColor", colors);
    mShaderVoxels.uploadAttrib("vFaces", faces);
    Vector3f voxelSize = mVoxels->voxelSize();
    mShaderVoxels.setUniform("voxelSize", voxelSize);
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