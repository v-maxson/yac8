#include <stdlib.h>
#include "stack.h"

yac_stack *yac_stack_new(const size_t size)
{
	yac_stack *stack = malloc(sizeof(yac_stack));
	if (stack == NULL) return NULL;

	stack->size = size;
	stack->top = -1;
	stack->data = (uint16_t *)malloc(size * sizeof(uint16_t));
	if (stack->data == NULL) {
		free(stack);
		return NULL;
	}

	return stack;
}

void yac_stack_del(yac_stack *stack)
{
	free(stack->data);
	free(stack);
}

bool yac_stack_push(yac_stack *stack, const uint16_t value)
{
	if (stack->top + 1 >= stack->size) return false;

	stack->top++;
	stack->data[stack->top] = value;
	return true;
}

uint16_t yac_stack_pop(yac_stack *stack)
{
	if (stack->top < 0) return 0;

	const uint16_t value = stack->data[stack->top];
	stack->top--;
	return value;
}

uint16_t yac_stack_peek(const yac_stack *stack)
{
	if (stack->top < 0) return 0;

	return stack->data[stack->top];
}