#ifndef VOXIGN_VOXELS_H
#define VOXIGN_VOXELS_H

#include "common.h"
#include "pixels.h"

#include <nanogui/glutil.h>


using namespace Eigen;

struct RGB : public Array3f {
public:
    typedef Array3f Base;

    RGB(float value = 0.0f) : Base(value, value, value) {}

    RGB(float r, float g, float b) : Base(r, g, b) {}

    template<typename Derived>
    RGB(const Eigen::ArrayBase<Derived> &p)
            : Base(p) {}

    template<typename Derived>
    RGB &operator=(const Eigen::ArrayBase<Derived> &p) {
        this->Base::operator=(p);
        return *this;
    }

    const float &r() const { return x(); }

    const float &g() const { return y(); }

    const float &b() const { return z(); }

    std::string toString() const {
        return tfm::format("[%.2f, %.2f, %.2f]", Base::coeff(0), Base::coeff(1), Base::coeff(2));
    }
};

class Layer : public Array<RGB, Dynamic, Dynamic, RowMajor> {
public:
    typedef Array<RGB, Dynamic, Dynamic, RowMajor> Base;

    Layer(const Vector2i &size = Vector2i(0, 0)) : Base(size.x(), size.y()) {}

    void savePNG(const std::string &name) {
        std::vector<unsigned char> image;
        image.resize(cols() * rows() * 4);
        for (unsigned i = 0; i < cols(); ++i) {
            for (unsigned j = 0; j < rows(); ++j) {
                image[4 * (i + j * cols()) + 0] = static_cast<unsigned char>((*this)(j,i).r() * 255);
                image[4 * (i + j * cols()) + 1] = static_cast<unsigned char>((*this)(j,i).g() * 255);
                image[4 * (i + j * cols()) + 2] = static_cast<unsigned char>((*this)(j,i).b() * 255);
                image[4 * (i + j * cols()) + 3] = 255;
            }
        }

        unsigned error = lodepng::encode(name, image, cols(), rows());

        if(error)
            fprintf(stderr, "Save PNG err: %d: %s\n", error, lodepng_error_text(error));
    }
};

class Voxels {
public:
    Voxels() {};
    Voxels(const Array3f &voxelShape, const Array3i &boundShape);
    ~Voxels();

    void setShader(const std::string &shader);

//    void resizeVoxels(const Array3f &shape);
    void resizeBounds(const Array3i &bounds, const Array3f &voxels);
    void resizeVolume(const Array3f &volume, const Array3f &voxels);

    bool ready();

    void clearVoxels();

    void renderLayer(unsigned long layer);
    void voxelizeLayer(unsigned long layer);
    void saveLayer(unsigned long layer);
    void saveVoxels(const std::string &folder);

    bool finished(unsigned long layer);
    bool finishedAll();

    Array2i layerSize() { return {mBounds.coeff(0), mBounds.coeff(1)}; }
    unsigned long layerCount() { return mVoxels.size(); }
    Array3f volume() { return mVolume; }
    Array3f voxelSize() { return mVoxelSize; }
    Array3f calcVolume() { return mVoxelSize.cwiseProduct(mBounds.cast<float>()); }
    Array3i calcBounds() { return mVolume.cwiseQuotient(mVoxelSize).ceil().cast<int>(); }
private:
    void resizeLayers(unsigned long layers);

    Array3f mVolume;
    Array3i mBounds;
    Array3f mVoxelSize;

    std::vector<Layer> mVoxels;
    std::vector<bool> mFinishedLayers;
    bool mFinished;

    nanogui::GLFramebuffer mFbo;
    Pixels mVoxignProgram;

};

void test_clear();
void test_finishedAll();
void test_resizeVolume();

#endif //VOXIGN_VOXELS_H
