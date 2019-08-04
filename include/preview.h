#ifndef VOXIGN_PREVIEW_H
#define VOXIGN_PREVIEW_H

#include "common.h"
#include "voxels.h"
#include "camera.h"
#include "trackball.h"

#include <nanogui/nanogui.h>

class Preview : public nanogui::GLCanvas {
public:
    Preview(nanogui::Widget* parent);
    ~Preview() override;

    void setVoxels(Voxels *voxels);
    void setLayer(float layer);

    void drawGL() override;

protected:
//    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual bool mouseMotionEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers);
//    virtual bool scrollEvent(const Eigen::Vector2i &p, const Eigen::Vector2f &rel);

private:
    inline bool isSolid(const RGB &voxel);
    void printVoxels();

    Voxels *mVoxels;
    float mLayer;
    int mSolidCount;
    int mInsideCount;

    nanogui::GLShader mShaderVoxels;
    nanogui::GLShader mShaderBounds;
    nanogui::GLShader mShaderAxis;
    nanogui::GLShader mShaderLayer;

    Eigen::Vector3f mRotation;
    Camera mCamera;
    enum TrackMode {
        TM_NO_TRACK=0, TM_ROTATE_AROUND
    };
    TrackMode mTrackMode;
    Trackball mTrackball;

    Eigen::Vector2i mMousePos;


    bool mReady = false;
    bool mWireframe = false;

    const RGB mEmpty = RGB(0.0);
};


#endif //VOXIGN_PREVIEW_H
