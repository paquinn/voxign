#include <voxels.h>
#include <fstream>


Voxels::Voxels() {
    // TODO: Find better solution to default values
    resizeBounds({10, 10, 10}, {1.0, 1.0, 1.0}), mOutside(0.0);
}

void Voxels::setShader(const std::string &shader) {
    mVoxignProgram.free();
    mVoxignProgram.init("slicer", shader);
}

void Voxels::resizeBounds(const Array3i &bounds, const Array3f &voxels) {
    // Equality comparison
    if (!mBounds.isApprox(bounds)) {
        mBounds = bounds;
        resizeLayers(layerCount());
        if (mFbo.ready()) mFbo.free();
        mFbo.init(layerSize(), 1);
    }
    mVoxelSize = voxels;
    mVolume = calcVolume();
    clearVoxels();
}

void Voxels::resizeVolume(const Array3f &volume, const Array3f &voxels) {
    mVolume = volume;
    mVoxelSize = voxels;
    // Above will be replaced by resizeBounds
    resizeBounds(calcBounds(), voxels);
}

void Voxels::clearVoxels() {
    for (auto it = mFinishedLayers.begin(); it < mFinishedLayers.end(); it++) {
        *it = false;
    }
    mNumFinished = 0;
}

void Voxels::renderLayer(unsigned long layerIndex) {
    if (ready()) {
        mVoxignProgram.bind();
        mVoxignProgram.setUniform("slice", float(layerIndex));

        // TODO: Don't need to set this every layer, set once outside
        Vector3f vol = volume();
        Vector3f res = bounds().cast<float>();
        mVoxignProgram.setUniform("resolution", res);
        mVoxignProgram.setUniform("volume", vol);

        mVoxignProgram.draw();
    } else {
        // TODO: Possibly throw error if trying to render without being ready
    }
}

void Voxels::voxelizeLayer() {
//    assert(layer == mNumFinished);
    if (!ready()) return; // TODO: This should also be an error

    auto start = std::chrono::system_clock::now();

    // TODO: Check if thise type of lazy resizing is a benefit over a more proactive approach
    auto size = layerSize();
    mVoxels.at(mNumFinished).resize(size.coeff(0), size.coeff(1));

    mFbo.bind();
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderLayer(mNumFinished);
    mFbo.release();

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    mFbo.bind();
    glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    glReadPixels(0, 0, layerSize().coeff(0), layerSize().coeff(1), GL_RGB, GL_FLOAT, mVoxels.at(mNumFinished).data());
    // TODO: Maybe change this to always output to a specificfolder
//    mFbo.downloadTGA(tfm::format("%s_%06d.tga", "download", mNumFinished));
    mFbo.release();

    mFinishedLayers.at(mNumFinished) = true;
    mNumFinished += 1;
}

bool Voxels::voxelizeLayers(int count) {
    if (finished()) {
        return false;
    } else {
        for (int i = 0; i < count && !finished(); ++i) {
            voxelizeLayer();
        }
        // TODO: This should only return true once when it has finished
        return finished();
    }
}

// Make sure these always happen together
void Voxels::resizeLayers(unsigned long layers) {
    mVoxels.resize(layers);
    mFinishedLayers.resize(layers);
}

bool Voxels::finishedLayer(unsigned long layer) {
    return mFinishedLayers.at(layer);
}

bool Voxels::finished() {
    return mNumFinished == layerCount();
}

int Voxels::progress() {
    return mNumFinished;
}

void Voxels::saveLayer(unsigned long layer) {
    if (finishedLayer(layer)) {
        std::string name = tfm::format("%06d.png", layer);
        mVoxels.at(layer).savePNG(name);
    } else {
        std::cout << "Layer not rendered " << layer << std::endl;
    }
}

void Voxels::saveVoxels(const std::string &folder) {
    if (finished()) {
        for (int i = 0; i < layerCount(); ++i) {
            std::string name = tfm::format("%s_%06d.png", folder, i);
            cout << "Saving: " << name << endl;
            mVoxels.at(i).savePNG(name);
        }
    } else {
        cout << "Not all layers rendered" << endl;
    }
}

const RGB& Voxels::index(int x, int y, int z) {
    if (z < 0 || z >= layerCount() ||
            x < 0 || x >= layerSize().coeff(0) ||
            y < 0 || y >= layerSize().coeff(1)) {
//        tfm::printfln("Outside bounds %s,%s,%s", x, y, z);
        return mOutside;
    } else {
        return mVoxels.at(z)(x, y);
    }
}

bool Voxels::ready() {
    return mVoxignProgram.ready();
}

Voxels::~Voxels() {
    mVoxignProgram.free();
}

//void test_clear() {
//    Voxels v;
//
//    v.resizeBounds(Array3i(10, 10, 5));
//    v.voxelizeLayer(1);
//    v.voxelizeLayer(3);
//    assert(v.finishedLayer(2) == false);
//    assert(v.finishedLayer(1) == true);
//    v.clearVoxels();
//    assert(v.finishedLayer(3) == false);
//}
//
//void test_finishedAll() {
//    Voxels v;
//
//    v.resizeBounds(Array3i(5,5,3));
//
//    assert(v.finished() == false);
//    v.voxelizeLayer(0);
//    assert(v.finished() == false);
//    v.voxelizeLayer(1);
//    v.voxelizeLayer(2);
//    assert(v.finished() == true);
//    v.clearVoxels();
//    assert(v.finished() == false);
//}
//
//void test_resizeVolume() {
//    Voxels v;
//    v.resizeVoxels(Array3f(0.3, 0.5, 0.1));
//    v.resizeVolume(Array3f(10, 10, 10));
//    assert(v.layerSize().coeff(0) == 4);
//    assert(v.layerSize().coeff(1) == 2);
//    assert(v.layerCount() == 10);
//}
