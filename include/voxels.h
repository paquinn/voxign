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

    // TODO: lodePNG compresses output by default, find way to output uncompressed files
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
        cout << "Saving png " << name << endl;
        if(error)
            fprintf(stderr, "Save PNG err: %d: %s\n", error, lodepng_error_text(error));
    }

    // TODO: This does not work correctly do to expecting bgra format
    void saveTGA(const std::string &name) {
        FILE *tga = fopen(name.c_str(), "wb");
        if (tga == nullptr) {
            tfm::printfln("File: %s could not be opened", name);
            return;
        }
        fputc(0, tga); /* ID */
        fputc(0, tga); /* Color map */
        fputc(2, tga); /* Image type */
        fputc(0, tga); fputc(0, tga); /* First entry of color map (unused) */
        fputc(0, tga); fputc(0, tga); /* Length of color map (unused) */
        fputc(0, tga); /* Color map entry size (unused) */
        fputc(0, tga); fputc(0, tga);  /* X offset */
        fputc(0, tga); fputc(0, tga);  /* Y offset */
        fputc(cols() % 256, tga); /* Width */
        fputc(cols() / 256, tga); /* continued */
        fputc(rows() % 256, tga); /* Height */
        fputc(rows() / 256, tga); /* continued */
        fputc(32, tga);   /* Bits per pixel */
        fputc(0x20, tga); /* Scan from top left */
        fwrite(data(), cols() * rows() * 4, 1, tga);
        fclose(tga);
    }
};

class Voxels {
public:
    Voxels();
    ~Voxels();

    void setShader(const std::string &shader);

    void resizeBounds(const Array3i &bounds, const Array3f &voxels);
    void resizeVolume(const Array3f &volume, const Array3f &voxels);

    bool ready();

    void clearVoxels();

    bool voxelizeLayers(int count);
    void saveVoxels(const std::string &folder);
    bool finished();
    int progress();

    Array2i layerSize() { return {mBounds.coeff(0), mBounds.coeff(1)}; }
    const int layerCount() { return mBounds.coeff(2); }

    Array3f volume() { return mVolume; }
    Array3f voxelSize() { return mVoxelSize; }
    Array3f calcVolume() { return mVoxelSize.cwiseProduct(mBounds.cast<float>()); }
    Array3i calcBounds() { return mVolume.cwiseQuotient(mVoxelSize).ceil().cast<int>(); }

    RGB index(int x, int y, int z);
private:
    bool finishedLayer(unsigned long layer);
    void renderLayer(unsigned long layer);
    void voxelizeLayer();
    void saveLayer(unsigned long layer);

    void resizeLayers(unsigned long layers);

    Array3f mVolume;
    Array3i mBounds;
    Array3f mVoxelSize;

    std::vector<Layer> mVoxels;
    std::vector<bool> mFinishedLayers;
    int mNumFinished;

    nanogui::GLFramebuffer mFbo;
    Pixels mVoxignProgram;

};

void test_clear();
void test_finishedAll();
void test_resizeVolume();

#endif //VOXIGN_VOXELS_H
