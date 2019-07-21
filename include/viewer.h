#ifndef VOXIGN_VIEWER_H
#define VOXIGN_VIEWER_H

#include "common.h"
#include "voxels.h"
#include "compiler.h"

#include <nanogui/screen.h>
#include <nanogui/glutil.h>

class Viewer : public nanogui::Screen {
public:
    Viewer();
    ~Viewer();

    virtual void drawContents();

    void reloadPython();

    void initializeGL();
    void initializeLayout();

    void setFile(const std::string &filename);

    void setVoxelSize(Eigen::Vector3f size);
    void setBounds(Eigen::Vector3i bounds);
    void setVolume(Eigen::Vector3f volume);

private:
//    nanogui::GLShader mPreviewMarcherProgram;
//    nanogui::GLShader mPreviewVoxelProgram;
//    nanogui::GLShader mPreviewSliceProgram;

    nanogui::GLShader mRenderProgram;
    Compiler mCompiler;
    Voxels mVoxels;


};
#endif //VOXIGN_VIEWER_H
