#pragma once

#include <stdint.h>

uint32_t jump_consistent_hash(uint64_t key, uint32_t num_buckets);
