#ifndef VOXIGN_VOXELS_H
#define VOXIGN_VOXELS_H

#include "common.h"
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
    Voxels() { mHasSDF = false; };
    Voxels(const Array3f &voxelShape, const Array3i &boundShape);
    ~Voxels();

    void setSDF(const std::string &sdf);
    void resizeVoxels(const Array3f &shape);
    void resizeBounds(const Array3i &shape);
    void resizeVolume(const Array3f &shape);

    void clearVoxels();

    void renderLayer(unsigned long layer);
    void voxelizeLayer(unsigned long layer);

    bool finished(unsigned long layer);
    bool finishedAll();

    void saveLayer(unsigned long layer);

    Array2i layerSize() { return mSize; }
    unsigned long layerCount() {return mVoxels.size(); }
    Array3f volume() { return mVoxelShape.cwiseProduct(mBounds.cast<float>()); }
    Array3f shape() { return mVoxelShape; }

private:
    void resizeLayers(unsigned long layers);

    bool mHasSDF;
    Array2i mSize;
    Array3i mBounds;
    Array3f mVoxelShape;

    std::vector<Layer> mVoxels;
    std::vector<bool> mFinishedLayers;
    bool mFinished;

    nanogui::GLFramebuffer mFbo;
    nanogui::GLShader mVoxignProgram;

};

void test_clear();
void test_finishedAll();
void test_resizeVolume();

#endif //VOXIGN_VOXELS_H
