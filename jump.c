#include "jump.h"

/* see https://arxiv.org/abs/1406.2294 */
int32_t jump_consistent_hash(uint64_t key, int32_t num_buckets)
{
	int64_t b = -1;
	int64_t j = 0;

	while (j < num_buckets) {
		b = j;
		key = key * 2862933555777941757ULL + 1;
		j = (b + 1) * ((double)(1LL << 31) / (double)((key >> 33) + 1));
	}

	return (int32_t)b;
}
