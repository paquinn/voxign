#include "preview.h"

using namespace nanogui;


Preview::Preview(Widget *parent)
        : GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f)) {
    mShaderVoxels.initFromFiles("voxels",
                                DATA_DIR"/shaders/voxels.vert",
                                DATA_DIR"/shaders/voxels.frag",
                                DATA_DIR"/shaders/voxels.geom");
    mShaderBounds.initFromFiles("bounds",
                                DATA_DIR"/shaders/bounds.vert",
                                DATA_DIR"/shaders/voxels.frag");
    mShaderAxis.initFromFiles("axis",
                              DATA_DIR"/shaders/bounds.vert",
                              DATA_DIR"/shaders/voxels.frag");

    mCamera = Camera();
    mTrackball.setCamera(&mCamera);
}

void Preview::setVoxels(Voxels *pVoxels) {
    // TODO: Might not be needed
//    if (mShaderVoxels.hasAttrib("vPosition")) {
//        mShaderVoxels.freeAttrib("vPosition");
//        mShaderVoxels.freeAttrib("vColor");
//        mShaderVoxels.freeAttrib("vFaces");
//    }

    mVoxels = pVoxels;
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

    auto start = std::chrono::system_clock::now();
    for (int z = 0; z < layers; ++z) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (isSolid(mVoxels->index(x, y, z))) {
                    int sides = 0;
                    sides |= isSolid(mVoxels->index(x + 1, y, z)) << 2;
                    sides |= isSolid(mVoxels->index(x - 1, y, z)) << 3;
                    sides |= isSolid(mVoxels->index(x, y + 1, z)) << 0;
                    sides |= isSolid(mVoxels->index(x, y - 1, z)) << 1;
                    sides |= isSolid(mVoxels->index(x, y, z + 1)) << 4;
                    sides |= isSolid(mVoxels->index(x, y, z - 1)) << 5;
                    if (sides == 0b111111) {
                        insideCount++;
                    } else {
                        int index = solidCount - insideCount;
                        // TODO: y and x must be flipped here, this has something to
                        // do with converting between coordinate systems. Further investigation
                        positions.col(index) << y, x, z;
                        colors.col(index) << x / float(width), y / float(height), z / float(layers);
                        faces.col(index) << sides;
                    }

                    solidCount++;
                }
            }
        }
    }

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<float> elapsed = end - start;

    tfm::printfln("Voxel loop %s seconds", elapsed.count());

    // TODO: Resize these matrix arrays before sending them to the gpu
    // TODO: Or consider adding them to a vector and then creating the matrix from .data()
    mShaderVoxels.bind();
    mShaderVoxels.uploadAttrib("vPosition", positions);
    mShaderVoxels.uploadAttrib("vColor", colors);
    mShaderVoxels.uploadAttrib("vFaces", faces);
    Vector3f voxelSize = mVoxels->voxelSize();
    mShaderVoxels.setUniform("voxelSize", voxelSize);
    Vector3f volume = mVoxels->volume();
    mShaderVoxels.setUniform("volume", volume);
//    mShaderVoxels.setUniform("voxelSize", Vector3f{1.0, 1.0, 1.0});

    mSolidCount = solidCount;
    mInsideCount = insideCount;

    tfm::printfln("Solid count: %s/%s", solidCount, voxelCount);
    tfm::printfln("Inside count: %s/%s", insideCount, voxelCount);


    MatrixXu boundIndices(3, 12); /* Draw a cube */
    boundIndices.col(0) << 0, 1, 3;
    boundIndices.col(1) << 3, 2, 1;
    boundIndices.col(2) << 3, 2, 6;
    boundIndices.col(3) << 6, 7, 3;
    boundIndices.col(4) << 7, 6, 5;
    boundIndices.col(5) << 5, 4, 7;
    boundIndices.col(6) << 4, 5, 1;
    boundIndices.col(7) << 1, 0, 4;
    boundIndices.col(8) << 4, 0, 3;
    boundIndices.col(9) << 3, 7, 4;
    boundIndices.col(10) << 5, 6, 2;
    boundIndices.col(11) << 2, 1, 5;

    MatrixXf boundPositions(3, 8);
    float dx = mVoxels->volume().coeff(0) / 2.0f;
    float dy = mVoxels->volume().coeff(1) / 2.0f;
    float dz = mVoxels->volume().coeff(2) / 2.0f;
    boundPositions.col(0) << -dx, dy, dz;
    boundPositions.col(1) << -dx, dy, -dz;
    boundPositions.col(2) << dx, dy, -dz;
    boundPositions.col(3) << dx, dy, dz;
    boundPositions.col(4) << -dx, -dy, dz;
    boundPositions.col(5) << -dx, -dy, -dz;
    boundPositions.col(6) << dx, -dy, -dz;
    boundPositions.col(7) << dx, -dy, dz;

    MatrixXf boundColors(3, 8);
    for (int i = 0; i < 8; ++i) {
        boundColors.col(i) << 1.0, 0.0, 1.0;
    }


    mShaderBounds.bind();
    mShaderBounds.uploadIndices(boundIndices);
    mShaderBounds.uploadAttrib("vPosition", boundPositions);
    mShaderBounds.uploadAttrib("vColor", boundColors);


    MatrixXu axisIndices(2, 3);
    axisIndices.col(0) << 0, 1;
    axisIndices.col(1) << 0, 2;
    axisIndices.col(2) << 0, 3;

    MatrixXf axisPositions(3, 4);
    float far = 1e6;
    axisPositions.col(0) << 0.0, 0.0, 0.0;
    axisPositions.col(1) << far, 0.0, 0.0;
    axisPositions.col(2) << 0.0, far, 0.0;
    axisPositions.col(3) << 0.0, 0.0, far;

    MatrixXf axisColors(3, 4);
    axisColors.col(0) << 0.0, 0.0, 0.0;
    axisColors.col(1) << 1.0, 0.0, 0.0;
    axisColors.col(2) << 0.0, 1.0, 0.0;
    axisColors.col(3) << 0.0, 0.0, 1.0;

    mShaderAxis.bind();
    mShaderAxis.uploadIndices(axisIndices);
    mShaderAxis.uploadAttrib("vPosition", axisPositions);
    mShaderAxis.uploadAttrib("vColor", axisColors);

    mReady = true;
}

void Preview::setLayer(float layer) {
    mLayer = layer;
}

void Preview::drawGL() {
    if (mReady) {
        if (mWireframe) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
        glEnable(GL_DEPTH_TEST);

        // TODO: Should only happen once
        mCamera.setViewport(mSize[0], mSize[1]);

        Matrix4f model = Matrix4f(Eigen::Affine3f::Identity().data());
        Matrix4f view = Matrix4f(mCamera.viewMatrix().data());
        Matrix4f projection = mCamera.projectionMatrix();
        Matrix4f mvp = projection * view * model;

        mShaderVoxels.bind();
        mShaderVoxels.setUniform("mvp", mvp);
        mShaderVoxels.drawArray(GL_POINTS, 0, mSolidCount - mInsideCount);

        if (!mWireframe) { glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }

        mShaderBounds.bind();
        mShaderBounds.setUniform("mvp", mvp);
        mShaderBounds.drawIndexed(GL_TRIANGLES, 0, 12);

        mShaderAxis.bind();
        mShaderAxis.setUniform("mvp", mvp);
        mShaderAxis.drawIndexed(GL_LINES, 0, 3);

        glDisable(GL_DEPTH_TEST);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

bool Preview::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers) {
    if (mReady) {
        if (down) {
            mMousePos = p;
            switch (button) {
                case GLFW_MOUSE_BUTTON_LEFT:
                    mTrackball.start(Trackball::Around);
                    mTrackball.track(mMousePos);
                    mTrackMode = TM_ROTATE_AROUND;
                    break;
                case GLFW_MOUSE_BUTTON_MIDDLE:
                    break;
                case GLFW_MOUSE_BUTTON_RIGHT:
                    break;
                default:
                    break;
            }
        } else {
            mTrackMode = TM_NO_TRACK;
        }
    }
    return true;
}

bool Preview::mouseMotionEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers) {
//    tfm::printfln("p coords: (%s, %s), trackmode: %s", p[0], p[1], mTrackMode);
//    cout << mCamera.orientationMatrix() << endl;
    if (mReady) {
        if (mTrackMode != TM_NO_TRACK) {
//            float dx = float(p[0] - mMousePos[0]) / float(mCamera.vpWidth());
//            float dy = -float(p[1] - mMousePos[1]) / float(mCamera.vpHeight());
            mTrackball.track(p);
        }
        mMousePos = p;
    }
    return true;
}

bool Preview::scrollEvent(const Eigen::Vector2i &p, const Eigen::Vector2f &rel) {
    if (mReady) {
        mCamera.zoom(rel[1] * 1.f);
    }
    return true;
}

inline bool Preview::isSolid(const RGB &voxel) {
    return voxel.r() > Epsilon && voxel.g() > Epsilon && voxel.b() > Epsilon;
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
    mShaderBounds.free();
    mShaderAxis.free();
    mShaderLayer.free();
}