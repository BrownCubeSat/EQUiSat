/*
 * flash_Stack.c
 *
 * Created: 11/1/2016 9:19:41 PM
 *  Author: jleiken
 */ 

// Basic implementation from
// http://groups.csail.mit.edu/graphics/classes/6.837/F04/cpp_notes/stack1.html
#include "flash_Stack.h"

flash_Stack* flash_Stack_Init()
{
	flash_Stack* S = pvPortMalloc(sizeof(flash_Stack));
	S->top_index = -1;
	S->bottom_index = -1;
	S->mutex = xSemaphoreCreateMutex();
	S->size = 0;
	
	for (int i = 0; i < FLASH_STACK_MAX; i++)
	{
		S->data[i] = pvPortMalloc(sizeof(flash_data_t));
	}
	
	return S;
}

flash_data_t* flash_Stack_Get(flash_Stack* S, int16_t n)
{
	xSemaphoreTake(S->mutex, (TickType_t) MUTEX_WAIT_TIME_TICKS);
	
	if (n < S->size)
	{
		return S->data[(S->top_index + n) % FLASH_STACK_MAX];
	}
	else
	{
		return NULL;
	}
	
	xSemaphoreGive(S->mutex);
}

// Overwrites the bottom value if need be
flash_data_t* flash_Stack_Stage(flash_Stack* S)
{
	if (S->size > 0)
	{
		xSemaphoreTake(S->mutex, (TickType_t) MUTEX_WAIT_TIME_TICKS);
	
		S->top_index = (S->top_index + 1) % FLASH_STACK_MAX;
	
		if (S->bottom_index == S->top_index)
		{
			S->bottom_index = (S->bottom_index + 1) % FLASH_STACK_MAX;
		}
		else
		{
			S->size++;
		
			if (S->bottom_index == -1)
			{
				S->bottom_index = 0;
			}
		}
	
		flash_data_t* staged_pointer = S->data[(S->top_index + 1) % FLASH_STACK_MAX];
		clear_existing_data(staged_pointer, sizeof(flash_data_t));
	
		xSemaphoreGive(S->mutex);
		return staged_pointer; // return pointer to staged data
	}
	else
	{
		return S->data[S->top_index + 1];
	}
}