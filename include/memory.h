#ifndef __YAC_MEMORY_H__
#define __YAC_MEMORY_H__

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define DEFINE_YAC_MEMORY_TYPE(type)						\
typedef struct yac_memory_##type {						\
	size_t size;								\
	type *data;								\
} yac_memory_##type;								\
yac_memory_##type yac_memory_##type##_new(size_t size);				\
void yac_memory_##type##_del(yac_memory_##type *mem);				\

typedef uint8_t u8;

DEFINE_YAC_MEMORY_TYPE(u8)	// For interpreter memory.
DEFINE_YAC_MEMORY_TYPE(bool)	// For display memory.


#endif
