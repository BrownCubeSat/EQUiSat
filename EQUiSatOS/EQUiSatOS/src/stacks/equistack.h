/*
 * equistack.h
 *
 * Created: 3/1/2017 8:43:43 PM
 *  Author: mcken
 */ 


#ifndef EQUISTACK_H_
#define EQUISTACK_H_
#include "equistacks.h"

typedef struct equistack
{
	int16_t     top_index;
	int16_t     bottom_index;
	int16_t     cur_size;
	int16_t		max_size;
	size_t		data_size;
	SemaphoreHandle_t mutex;
	void*		data[];
} equistack;

equistack* equistack_Init(equistack* S, void* data, size_t data_size, uint16_t max_size);
void* equistack_Get(equistack* S, int16_t n);
void* equistack_Initial_Stage(equistack* S);
void* equistack_Stage(equistack* S);

#endif /* EQUISTACK_H_ */