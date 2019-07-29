#include "pixels.h"

bool Pixels::init(const std::string &name, const std::string &fragment_str) {

    auto file_to_string = [](const std::string &filename) -> std::string {
        if (filename.empty())
            return "";
        std::ifstream t(filename);
        return std::string((std::istreambuf_iterator<char>(t)),
                           std::istreambuf_iterator<char>());
    };

    mReady = nanogui::GLShader::init(name, file_to_string(mVertexShader), fragment_str);

    if (mReady) {
        nanogui::MatrixXu indices(3, 2);
        indices.col(0) << 0, 1, 2;
        indices.col(1) << 2, 1, 3;

        Eigen::MatrixXf positions(3, 4);
        positions.col(0) << -1, -1, 0;
        positions.col(1) << 1, -1, 0;
        positions.col(2) << -1, 1, 0;
        positions.col(3) << 1, 1, 0;

        // bind the shader and upload vertex positions and indices
        bind();
        uploadIndices(indices);
        uploadAttrib(mPositionAttrib, positions);

    }
    return ready();
}

void Pixels::draw() {
    if (!ready()) {
        throw std::runtime_error{tfm::format("Draw was called before shader was successfully initialized.")};
    }
    glDisable(GL_DEPTH_TEST);
    drawIndexed(GL_TRIANGLES, 0, 2);
}

bool Pixels::ready() {
    return mReady;
}

const std::string Pixels::mVertexShader = DATA_DIR"/shaders/pass.glsl";
const std::string Pixels::mPositionAttrib = "positions";