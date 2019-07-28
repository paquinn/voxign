#include "viewer.h"

#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/button.h>

Viewer::Viewer() :
        // TODO: Change to resizeable back to true
        nanogui::Screen(nanogui::Vector2i(800, 800), "Voxign", false),
        mVoxels(),
        mCompiler()
{
    using namespace nanogui;

    mToolBar = new Window(this, "Test");
    mToolBar->setPosition(Vector2i(15, 15));
    mToolBar->setLayout(new BoxLayout{Orientation::Vertical, Alignment::Fill});

    {
        Button *voxelize = new Button(mToolBar, "Voxelize");
        voxelize->setCallback([this]() {
            tfm::printfln("Voxelizing %s layers", mVoxels.layerCount());
//            for (int i = 0; i < mVoxels.layerCount(); ++i) {
//                cout << "Layer " << i << endl;
//                mVoxels.voxelizeLayer(i);
//            }
            mVoxels.voxelizeLayers(100000);
            mPreview->setVoxels(&mVoxels);
//            cout << mVoxels.index(0, 0, 0).toString() << endl;
//            cout << mVoxels.index(50, 50, 50).toString() << endl;
        });

        Button *save = new Button(mToolBar, "Save");
        save->setCallback([this]() {
            tfm::printfln("Saving %s layers", mVoxels.layerCount());
            mVoxels.saveVoxels("test_folder");
        });
    }

    {
        Window *previewWindow = new Window(this, "Preview");
        previewWindow->setPosition(Vector2i(15, 15));
        previewWindow->setLayout(new GroupLayout());

        mPreview = new Preview(previewWindow);
        mPreview->setBackgroundColor({100, 100, 100, 255});
        mPreview->setSize({400, 400});
    }

    performLayout();
}

void Viewer::setInputFile(const std::string &filename) {
    mCompiler.load(filename);
    mVoxels.setShader(mCompiler.reload("sliceFramework"));
    mRenderProgram.init("render", mCompiler.reload("renderFramework"));
}

void Viewer::setOutputFolder(const std::string &foldername) {
    mOutputFolder = foldername;
}

void Viewer::setBounds(Eigen::Array3i bounds, Eigen::Array3f voxels) {
    mVoxels.resizeBounds(bounds, voxels);
}

void Viewer::setVolume(Eigen::Array3f volume, Eigen::Array3f voxels) {
    mVoxels.resizeVolume(volume, voxels);
}

void Viewer::drawContents() {
    using namespace nanogui;

    if (mRenderProgram.ready()) {
        mRenderProgram.bind();
        mRenderProgram.setUniform("resolution", Vector2f{mFBSize[0], mFBSize[1]});
        mRenderProgram.setUniform("mouse", Vector2f{mMousePos[0], mMousePos[1]}, false);
        mRenderProgram.draw();
    }
}

Viewer::~Viewer() {
    mRenderProgram.free();
}