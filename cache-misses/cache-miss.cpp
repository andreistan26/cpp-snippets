#include <random>
#include <sys/mman.h>
#include <assert.h>

#define ALIGN_DOWN(X, SIZE) ((X) & ~((SIZE) - 1))

constexpr uint64_t data_size = 1ULL << 32;
constexpr uint64_t cache_line_accesses = 1 << 28;
constexpr uint64_t chunk_size = 1 << 10;
constexpr uint64_t cache_line_size = 64;

int main() {
    char *data = static_cast<char *>(mmap(NULL, data_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, 0, 0));
    assert(data != nullptr);

    constexpr uint64_t chunks = cache_line_accesses / chunk_size;
    for (uint64_t chunk_idx = 0; chunk_idx < chunks; chunk_idx ++) {
        constexpr uint16_t cache_lines_per_chunk = chunk_size / cache_line_size;

		/* Choose a random chunk */
		uint64_t chunk_start = ALIGN_DOWN(rand() % data_size, chunk_size);
		uint64_t read_pos = chunk_start;

		if (chunk_idx % 2 == 1) {
			/* Touch all of the cache lines in the chunk sequentially */
			for (uint64_t i = 0; i < chunk_size; i += 1) {
				data[read_pos++] = 1;
			}
		} else {
			/* Touch cache lines from the chunk randomly */
			for (uint64_t i = 0; i < chunk_size; i += 64) {
				uint64_t read_pos = chunk_start + ALIGN_DOWN(rand() % chunk_size, cache_line_size);
				/* Touch the whole cache line in order to be consistent with the test above */
				for (uint64_t j = 0; j < 64; j++)
					data[read_pos++] = 1;
			}
		}
    }
}
