/*
 * battery_charging_tester.h
 *
 * Created: 11/2/2017 8:30:26 PM
 *  Author: rj16
 */ 

#ifndef BATTERY_CHARGING_TESTER_H_
#define BATTERY_CHARGING_TESTER_H_

#include "rtos_tasks/rtos_tasks.h"
#include "rtos_tasks/battery_charging_task.h"

void run_core_logic_unit_tests(void);
void run_convergence_tests(void);
void run_battery_charging_tests(void);

#endif