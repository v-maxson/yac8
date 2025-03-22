#include "memory.h"

#define DEFINE_YAC_MEMORY_TYPE_IMPL(type)							\
yac_memory_##type yac_memory_##type##_new(size_t size) {					\
	yac_memory_##type mem;									\
	mem.size = size;									\
	mem.data = (type *)malloc(size * sizeof(type));						\
	return mem;										\
}												\
void yac_memory_##type##_del(yac_memory_##type *mem) {						\
	free(mem->data);									\
}

DEFINE_YAC_MEMORY_TYPE_IMPL(u8)
DEFINE_YAC_MEMORY_TYPE_IMPL(u32)