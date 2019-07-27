#ifndef VOXIGN_PREVIEW_H
#define VOXIGN_PREVIEW_H

#include "common.h"

#include <nanogui/nanogui.h>

class Preview : public nanogui::GLCanvas {
public:
    Preview(nanogui::Widget* parent);
    ~Preview() override;

    void drawGL() override;

private:
    nanogui::GLShader mShaderVoxels;
    nanogui::GLShader mShaderBounds;

    Eigen::Vector3f mRotation;

};


#endif //VOXIGN_PREVIEW_H
