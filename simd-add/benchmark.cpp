#include <benchmark/benchmark.h>
#include <iostream>
#include <memory>
#include "vec_add.h"

template<typename T,  std::size_t Alignment>
struct aligned_array_t {
	T* data;

	aligned_array_t(std::size_t N) {
		data = static_cast<T *>(aligned_alloc(Alignment, N * sizeof(T)));
	}
	T& operator[](std::size_t i) { return data[i]; }
	const T& operator[](std::size_t i) const { return data[i]; }
	~aligned_array_t() { std::free(data); }
};

template<vec_add<float> F, std::uint64_t Alignment>
static void bm_vec_add_aligned(benchmark::State& state) {
    const uint64_t size = state.range(0);
 
	aligned_array_t<float, Alignment> a(size);
	aligned_array_t<float, Alignment> b(size);
	aligned_array_t<float, Alignment> result(size);

    for (uint64_t i = 0; i < size; i++) {
		a[i] = 1.0f;
		b[i] = 2.0f;
    }

    for (auto _ : state) {
		F(result.data, a.data, b.data, size);
        benchmark::DoNotOptimize(result);
		benchmark::ClobberMemory();
    }
    
    state.SetItemsProcessed(state.iterations() * size);
}

BENCHMARK(bm_vec_add_aligned<vec_add_avx, 32>)->
	RangeMultiplier(2)->Range(256, 8192)->Unit(benchmark::kMicrosecond);
BENCHMARK(bm_vec_add_aligned<vec_add_avx, 1>)->
	RangeMultiplier(2)->Range(256, 8192)->Unit(benchmark::kMicrosecond);
BENCHMARK(bm_vec_add_aligned<vec_add_native, 1>)->
	RangeMultiplier(2)->Range(256, 8192)->Unit(benchmark::kMicrosecond);

BENCHMARK_MAIN();
