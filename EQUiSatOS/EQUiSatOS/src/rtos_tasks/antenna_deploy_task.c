/*
 * antenna_deploy_task.c
 *
 * Created: 9/21/2017 20:36:14
 *  Author: mcken
 */ 

#include "rtos_tasks.h"
#include "../config/proc_pins.h"
#include "../processor_drivers/PWM_Commands.h"
#include "../runnable_configurations/antenna_pwm.h"

static int num_tries = 0;

int num_tries_ant_deploy(void) {
	return num_tries;
}

void antenna_deploy_task(void *pvParameters) {
	// delay to offset task relative to others, then start
	vTaskDelay(ANTENNA_DEPLOY_TASK_FREQ_OFFSET);
	TickType_t prev_wake_time = xTaskGetTickCount();
	
	init_task_state(ANTENNA_DEPLOY_TASK); // suspend or run on boot

	for( ;; )
	{		
		// only try quickly in ANTENNA_DEPLOY state, otherwise try more periodically
		if (get_sat_state() == ANTENNA_DEPLOY) {
			vTaskDelayUntil(&prev_wake_time, ANTENNA_DEPLOY_TASK_FREQ / portTICK_PERIOD_MS);
		} else {
			vTaskDelayUntil(&prev_wake_time, ANTENNA_DEPLOY_TASK_LESS_FREQ / portTICK_PERIOD_MS);
		}
		
		// report to watchdog
		report_task_running(ANTENNA_DEPLOY_TASK);
		
		if (true /* TODO: LiON is sufficiently charged*/) {
			if (xSemaphoreTake(critical_action_mutex, CRITICAL_MUTEX_WAIT_TIME_TICKS))
			{
				try_pwm_deploy(P_ANT_DRV1, P_ANT_DRV1_MUX, PWM_LENGTH_MS, 1);
				try_pwm_deploy(P_ANT_DRV2, P_ANT_DRV2_MUX, PWM_LENGTH_MS, 2);
				try_pwm_deploy(P_ANT_DRV3, P_ANT_DRV3_MUX, PWM_LENGTH_MS, 3);
				
				xSemaphoreGive(critical_action_mutex);
			} else {
				log_error(ELOC_ANTENNA_DEPLOY, ECODE_CRIT_ACTION_MUTEX_TIMEOUT, true);
			}
			num_tries++;
		}
	}
	
	// delete this task if it ever breaks out
	vTaskDelete(NULL);
}