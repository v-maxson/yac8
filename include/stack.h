#ifndef __YAC_STACK_H__
#define __YAC_STACK_H__

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// A stack of 16-bit unsigned integers.
typedef struct yac_stack {
	size_t size;
	int32_t top;
	uint16_t *data;
} yac_stack;

yac_stack *yac_stack_new(const size_t size);
void yac_stack_del(yac_stack *stack);
bool yac_stack_push(yac_stack *stack, const uint16_t value);
uint16_t yac_stack_pop(yac_stack *stack);
uint16_t yac_stack_peek(const yac_stack *stack);

#endif
