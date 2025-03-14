#include <cstdint>
#include <immintrin.h>

void vec_add_avx(float *result, const float *a, const float *b, const std::size_t size) {
	__m256 *result_v = (__m256 *)result;

	constexpr std::size_t stride = sizeof(__m256) / sizeof(float);
	const std::size_t vec_size = size / (sizeof(__m256) / sizeof(float));

	for (std::size_t i = 0; i < vec_size; i++) {
		__m256 a_i = _mm256_loadu_ps(a + i * stride);
		__m256 b_i = _mm256_loadu_ps(b + i * stride);
		__m256 res_i = _mm256_add_ps(a_i, b_i);
		_mm256_storeu_ps(result + i * stride, res_i);
	}
}

void vec_add_native(float *result, const float *a, const float *b, const std::size_t size) {
	for (std::size_t i = 0; i < size; i++) {
		result[i] = a[i] + b[i];
	}
}
