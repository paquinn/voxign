#ifndef VOXIGN_VIEWER_H
#define VOXIGN_VIEWER_H

#include "common.h"
#include "voxels.h"

#include <nanogui/screen.h>
#include <nanogui/glutil.h>

class Viewer : public nanogui::Screen {
public:
    Viewer();
    ~Viewer();

    virtual void drawContents();

    void loadPython(const std::string& filename);
    void reloadPython();

    void initializeGL();
    void initializeLayout();

private:
    nanogui::GLShader mPreviewMarcherProgram;
    nanogui::GLShader mPreviewVoxelProgram;
    nanogui::GLShader mPreviewSliceProgram;

    std::string mCurrentFile;
    Voxels mVoxels;


};
#endif //VOXIGN_VIEWER_H
