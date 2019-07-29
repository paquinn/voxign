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
    // TODO: BUG: Sending ints directly over causes their bits to be converted to float along the way
    MatrixXf faces(1, voxelCount);
    int solidCount = 0;
    int insideCount = 0;
    int layers = mVoxels->layerCount();
    Array2i size = mVoxels->layerSize();
    int width = size.coeff(0);
    int height = size.coeff(1);

    for (int z = 0; z < layers; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (isSolid(mVoxels->index(x, y, z))) {
                    int sides = 0;
                    sides |= isSolid(mVoxels->index(x + 1, y, z)) << 0;
                    sides |= isSolid(mVoxels->index(x - 1, y, z)) << 1;
                    sides |= isSolid(mVoxels->index(x, y + 1, z)) << 2;
                    sides |= isSolid(mVoxels->index(x, y - 1, z)) << 3;
                    sides |= isSolid(mVoxels->index(x, y, z + 1)) << 4;
                    sides |= isSolid(mVoxels->index(x, y, z - 1)) << 5;
                    if (sides == 0b111111) {
                        insideCount++;
                    } else {
                        int index = solidCount - insideCount;
                        positions.col(index) << x - layers/2, y - height/2, z - width/2;
                        colors.col(index) << 1.0, 1.0, 1.0;
                        faces.col(index) << sides;
                    }

                    solidCount++;
                }
            }
        }
    }

    // TODO: Resize these matrix arrays before sending them to the gpu
    // TODO: Or consider adding them to a vector and then creating the matrix from .data()
    mShaderVoxels.bind();

    mShaderVoxels.uploadAttrib("vPosition", positions);
    mShaderVoxels.uploadAttrib("vColor", colors);
    mShaderVoxels.uploadAttrib("vFaces", faces);
    Vector3f voxelSize = mVoxels->voxelSize();
    mShaderVoxels.setUniform("voxelSize", voxelSize);
    mSolidCount = solidCount;
    mInsideCount = insideCount;

    tfm::printfln("Solid count: %s/%s", solidCount, voxelCount);
    tfm::printfln("Inside count: %s/%s", insideCount, voxelCount);
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

        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        glEnable(GL_DEPTH_TEST);
        mShaderVoxels.drawArray(GL_POINTS, 0, mSolidCount - mInsideCount);
        glDisable(GL_DEPTH_TEST);
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

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