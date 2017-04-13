/*
 * init_rtos_tasks.h
 *
 * Created: 10/4/2016 8:47:48 PM
 *  Author: mckenna
 */ 

#ifndef INIT_RTOS_TASKS_H
#define INIT_RTOS_TASKS_H

#include "rtos_tasks.h"
#include "rtos_task_frequencies.h"
#include "struct_tests.h"
#include "stacks/equistack.h"

/* Global satellite state */
int8_t CurrentState;

/* Task handles for starting and stopping */
TaskHandle_t watchdog_task_handle; // Should we have this?
TaskHandle_t antenna_deploy_task_handle;
TaskHandle_t battery_charging_task_handle; // Should we have this?
TaskHandle_t flash_activate_task_handle;
TaskHandle_t transmit_task_handle;

TaskHandle_t current_data_task_handle;
TaskHandle_t current_data_low_power_task_handle;
TaskHandle_t flash_data_task_handle;
TaskHandle_t transmit_data_task_handle;
TaskHandle_t attitude_data_task_handle;

/* List (series of bits) of whether a given task is resuming from suspension */
uint8_t TaskSuspendStates;

void runit_2(void);

void set_state_hello_world(void);
void set_state_idle(void);
void set_state_low_power(void);

#endif