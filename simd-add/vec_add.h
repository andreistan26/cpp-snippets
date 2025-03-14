#pragma once

#include <cstdint>

template<typename T>
using vec_add = void (*)(T*, const T*, const T*, const std::size_t);

void vec_add_native(float* res, const float* a, const float* b, const uint64_t size);
void vec_add_avx(float *res, const float *a, const float *b, const uint64_t size);
