#include <voxels.h>
#include <fstream>

Voxels::Voxels(const Array3f &voxelShape, const Array3i &boundShape) {
    resizeVoxels(voxelShape);
    resizeBounds(boundShape);

    mHasSDF = false;
}

void Voxels::setShader(const std::string &shader) {
    mHasSDF = true;

    mVoxignProgram.free();
    mVoxignProgram.init("slicer", shader);

    Vector3f vol = volume();
    Vector3f res = mBounds.cast<float>();
    mVoxignProgram.setUniform("resolution", res);
    mVoxignProgram.setUniform("volume", vol);

}


void Voxels::resizeVoxels(const Array3f &shape) {
    mVoxelShape = shape;
    clearVoxels();
}

void Voxels::resizeBounds(const Array3i &shape) {
    unsigned long width = shape.x();
    unsigned long height = shape.y();

    unsigned long layers = shape.z();
    resizeLayers(layers);

    mSize = Array2i(width, height);
    mBounds = shape;

    if (mFbo.ready()) mFbo.free();
    mFbo.init(mSize, 1);

    clearVoxels();
}

void Voxels::resizeVolume(const Array3f &shape) {
    resizeBounds(shape.cwiseQuotient(mVoxelShape).ceil().cast<int>());
}

void Voxels::clearVoxels() {
    for (auto it = mFinishedLayers.begin(); it < mFinishedLayers.end(); it++) {
        *it = false;
    }
}

void Voxels::renderLayer(unsigned long layerIndex) {
    mVoxignProgram.bind();

    // TODO: set more uniforms here
    float layer = 0.0f;

    mVoxignProgram.setUniform("slice", layer);
    mVoxignProgram.draw();
}

void Voxels::voxelizeLayer(unsigned long layer) {
    if (!mHasSDF) return;
    auto start = std::chrono::system_clock::now();

    mFbo.bind();
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderLayer(layer);
    mFbo.release();

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    mFbo.bind();
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glReadPixels(0, 0, mSize.x(), mSize.y(), GL_RGB, GL_FLOAT, mVoxels.at(layer).data());
    mFbo.release();

    mFinishedLayers.at(layer) = true;
}

// Make sure these always happen together
void Voxels::resizeLayers(unsigned long layers) {
    mVoxels.resize(layers);
    mFinishedLayers.resize(layers);
}

bool Voxels::finished(unsigned long layer) {
    return mFinishedLayers.at(layer);
}

bool Voxels::finishedAll() {
    for (bool finished : mFinishedLayers) {
        if (!finished) return false;
    }
    return true;
}

void Voxels::saveLayer(unsigned long layer) {
    if (finished(layer)) {
        std::string name = tfm::format("%06d.png", layer);
        mVoxels.at(layer).savePNG(name);
    } else {
        std::cout << "Layer not rendered " << layer << std::endl;
    }
}

Voxels::~Voxels() {
    mVoxignProgram.free();
}

void test_clear() {
    Voxels v;

    v.resizeBounds(Array3i(10, 10, 5));
    v.voxelizeLayer(1);
    v.voxelizeLayer(3);
    assert(v.finished(2) == false);
    assert(v.finished(1) == true);
    v.clearVoxels();
    assert(v.finished(3) == false);
}

void test_finishedAll() {
    Voxels v;

    v.resizeBounds(Array3i(5,5,3));

    assert(v.finishedAll() == false);
    v.voxelizeLayer(0);
    assert(v.finishedAll() == false);
    v.voxelizeLayer(1);
    v.voxelizeLayer(2);
    assert(v.finishedAll() == true);
    v.clearVoxels();
    assert(v.finishedAll() == false);
}

void test_resizeVolume() {
    Voxels v;
    v.resizeVoxels(Array3f(0.3, 0.5, 0.1));
    v.resizeVolume(Array3f(10, 10, 10));
    assert(v.layerSize().coeff(0) == 4);
    assert(v.layerSize().coeff(1) == 2);
    assert(v.layerCount() == 10);
}
