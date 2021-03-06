#ifndef VOXIGN_COMMON_H
#define VOXIGN_COMMON_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <string.h>
#include <Eigen/Core>
#include <Eigen/Geometry>
#include <tinyformat.h>
#include <lodepng.h>
#include <chrono>
#include <tuple>
#include <args.hxx>
#include <fstream>


#ifndef VOXIGN_VERSION
#   define VOXIGN_VERSION "undefined"
#endif

#define Epsilon 1e-4f

#define PI           3.14159265358979323846f
#define INV_PI       0.31830988618379067154f
#define INV_TWOPI    0.15915494309189533577f
#define INV_FOURPI   0.07957747154594766788f
#define SQRT_TWO     1.41421356237309504880f
#define INV_SQRT_TWO 0.70710678118654752440f

typedef std::tuple<int, int, int> tuple3i;
typedef std::tuple<float, float, float> tuple3f;

template<typename T>
struct TupleReader {
    void operator()(const std::string &name, const std::string &value, std::tuple<T, T, T> &destination) {
        const std::string errorMessage = tfm::format("Tuple not formatted correctly, needs three comma separated values.");
        std::istringstream is(value);
        if (!is) { throw args::ParseError(errorMessage); } else { is >> std::get<0>(destination); }
        if (!is) { throw args::ParseError(errorMessage); } else { is.get(); }
        if (!is) { throw args::ParseError(errorMessage); } else { is >> std::get<1>(destination); }
        if (!is) { throw args::ParseError(errorMessage); } else { is.get(); }
        if (!is) { throw args::ParseError(errorMessage); } else { is >> std::get<2>(destination); }
        if (!is) { throw args::ParseError(errorMessage); }
    }
};

//std::istream& operator>>(std::istream& is, std::tuple<T, T, T>& tuple) {
//    const std::string errorMessage = tfm::format("Tuple not formatted correctly, needs three comma separated values.");
//    if (!is) { throw args::ParseError(errorMessage); } else { is >> std::get<0>(tuple); }
//    if (!is) { throw args::ParseError(errorMessage); } else { is.get(); }
//    if (!is) { throw args::ParseError(errorMessage); } else { is >> std::get<1>(tuple); }
//    if (!is) { throw args::ParseError(errorMessage); } else { is.get(); }
//    if (!is) { throw args::ParseError(errorMessage); } else { is >> std::get<2>(tuple); }
//    if (!is) { throw args::ParseError(errorMessage); }
//    return is;
//}

template<typename T>
Eigen::Matrix<T, 3, 1> castTuple(std::tuple<T, T, T> tuple);

// TODO: this should be replaced by above templated function
Eigen::Array3f tupleToArray3f(std::tuple<float, float, float> tuple);
Eigen::Array3i tupleToArray3i(std::tuple<int, int, int> tuple);

// TODO: Also templates, see above
std::string a3f(Eigen::Array3f arr);
std::string a3i(Eigen::Array3i arr);

using std::cout;
using std::cerr;
using std::endl;

#endif //VOXIGN_COMMON_H
