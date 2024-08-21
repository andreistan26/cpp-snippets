#include <vector>
#include <cstdint>
#include <thread>
#include <atomic>

/* simple sharing of an atomic integer */
void share_atomic_increment(int64_t num_threads, int64_t increment_count) {

	int64_t increment_per_thread = increment_count / num_threads;

	/* sharing the same cacheline between all threads 
	 * modifying that cacheline will invalidate it in another
	 * cpu cache, resulting in fetching from L3 or RAM
	 * */
	std::atomic<int64_t> var;
	std::vector<std::thread> threads;

	for (int64_t i = 0; i < num_threads; i++) {
		threads.emplace_back([&var](int64_t iter) {
			while (iter--) {
				var++;
			}
		}, increment_per_thread);
	}

	for (auto &thread : threads)
		thread.join();
}

int main(int argc, char **argv) {
	share_atomic_increment(16, 1L << 28);
}
