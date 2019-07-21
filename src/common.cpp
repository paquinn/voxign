#include "common.h"

template <typename T>
Eigen::Matrix<T, 3, 1> castTuple(std::tuple<T, T, T> tuple){
    return Eigen::Matrix<T, 3, 1>(std::get<0>(tuple), std::get<1>(tuple), std::get<2>(tuple));
};