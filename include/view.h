#ifndef VOXIGN_VIEW_H
#define VOXIGN_VIEW_H

#include <nanogui/nanogui.h>
#include "common.h"
#include "camera.h"
#include "trackball.h"

class View : public nanogui::GLCanvas {
public:
    View(nanogui::Widget* parent);

    Eigen::Vector3f cameraPosition() { return mCamera.position(); }
    Eigen::Matrix3f cameraOrientation() { return mCamera.orientationMatrix(); }

protected:

    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers);
    virtual bool mouseMotionEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers);
    virtual bool scrollEvent(const Eigen::Vector2i &p, const Eigen::Vector2f &rel);

private:
    Camera mCamera;
    enum TrackMode {
        TM_NO_TRACK=0, TM_ROTATE_AROUND
    };
    TrackMode mTrackMode;
    Trackball mTrackball;
    Eigen::Vector2i mMousePos;
};

#endif //VOXIGN_VIEW_H
