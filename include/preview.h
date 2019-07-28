#ifndef VOXIGN_PREVIEW_H
#define VOXIGN_PREVIEW_H

#include "common.h"
#include "voxels.h"

#include <nanogui/nanogui.h>

class Preview : public nanogui::GLCanvas {
public:
    Preview(nanogui::Widget* parent);
    ~Preview() override;

    void setVoxels(Voxels *voxels);
    void setLayer(float layer);
    void setViewInterval(int start, int end);

    void drawGL() override;

private:
    bool isSolid(RGB voxel);
    void printVoxels();

    Voxels *mVoxels;
    int mStart;
    int mEnd;
    float mLayer;
    int mSolidCount;

    nanogui::GLShader mShaderVoxels;
    nanogui::GLShader mShaderBounds;
    nanogui::GLShader mShaderLayer;

    Eigen::Vector3f mRotation;

    bool mReady = false;

    const RGB mEmpty = RGB(0.0);
};


#endif //VOXIGN_PREVIEW_H
