#include "common.h"

template <typename T>
Eigen::Matrix<T, 3, 1> castTuple(std::tuple<T, T, T> tuple){
    return Eigen::Matrix<T, 3, 1>(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
};

Eigen::Array3f tupleToArray3f(std::tuple<float, float, float> tuple) {
    return {std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)};

};

Eigen::Array3i tupleToArray3i(std::tuple<int, int, int> tuple) {
    return {std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple)};

};

std::string a3f(Eigen::Array3f arr) {
    return tfm::format("[%s %s %s]", arr.coeff(0), arr.coeff(1), arr.coeff(2));
}

std::string a3i(Eigen::Array3i arr) {
    return tfm::format("[%s %s %s]", arr.coeff(0), arr.coeff(1), arr.coeff(2));
}