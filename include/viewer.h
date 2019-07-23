#ifndef VOXIGN_VIEWER_H
#define VOXIGN_VIEWER_H

#include "common.h"
#include "voxels.h"
#include "compiler.h"
#include "pixels.h"

#include <nanogui/screen.h>
#include <nanogui/glutil.h>

class Viewer : public nanogui::Screen {
public:
    Viewer();
    ~Viewer();

    void drawContents() override;

//    void reloadPython();
//
//    void initializeGL();
//    void initializeLayout();

    void setInputFile(const std::string &filename);
    void setOutputFolder(const std::string &foldername);

    void setVoxelSize(Eigen::Array3f size);
    void setBounds(Eigen::Array3i bounds);
    void setVolume(Eigen::Array3f volume);

private:
//    nanogui::GLShader mPreviewMarcherProgram;
//    nanogui::GLShader mPreviewVoxelProgram;
//    nanogui::GLShader mPreviewSliceProgram;

    std::string mOutputFolder;
    Pixels mRenderProgram;
    Compiler mCompiler;
    Voxels mVoxels;


};
#endif //VOXIGN_VIEWER_H
