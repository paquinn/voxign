#ifndef VOXIGN_PIXELS_H
#define VOXIGN_PIXELS_H

#include "common.h"
#include <nanogui/glutil.h>

class Pixels : public nanogui::GLShader {
public:
    virtual bool init(const std::string &name, const std::string &fragment_str);
    virtual void draw();

    bool ready();

private:
    using nanogui::GLShader::initFromFiles;
    using nanogui::GLShader::drawIndexed;
    using nanogui::GLShader::drawArray;

    static const std::string mVertexShader;
    static const std::string mPositionAttrib;

    bool mReady = false;
};

#endif //VOXIGN_PIXELS_H
