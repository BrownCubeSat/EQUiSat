/*
 * idle_data_task.c
 *
 * Created: 9/21/2017 20:42:19
 *  Author: mcken
 */ 

#include "rtos_tasks.h"

/* Data Read Tasks */
void idle_data_task(void *pvParameters)
{
	// initialize xNextWakeTime onces
	TickType_t xNextWakeTime = xTaskGetTickCount();										
	
	// initialize first struct
	idle_data_t *current_struct = (idle_data_t*) equistack_Initial_Stage(&idle_readings_equistack);
	assert(current_struct != NULL); // TESTING
	current_struct->timestamp = get_current_timestamp();
	
	init_task_state(IDLE_DATA_TASK); // suspend or run on boot
	
	for( ;; )
	{	
		// block for a time based on a frequency, determined by whether we're in IDLE or LOW_POWER mode.
		// (Note: changes to the frequency can be delayed in taking effect by as much as the past frequency...)
		if (CurrentState == LOW_POWER) {
			vTaskDelayUntil( &xNextWakeTime, IDLE_DATA_LOW_POWER_TASK_FREQ / portTICK_PERIOD_MS);
		} else {
			vTaskDelayUntil( &xNextWakeTime, IDLE_DATA_TASK_FREQ / portTICK_PERIOD_MS);
		}
		
		// report to watchdog
		report_task_running(IDLE_DATA_TASK);
		
		// clear any previous suspend flags
		check_if_suspended_and_update(IDLE_DATA_TASK);
		
		// set start timestamp
		current_struct->timestamp = get_current_timestamp();
		
		// add all sensors to batch
		// read radio temp first because it takes a while to write & reset the radio
		read_radio_temp_batch(			&(current_struct->radio_temp_data));
	
		read_bat_temp_batch(			current_struct->bat_temp_data); 
		read_radio_volts_batch(			current_struct->radio_volts_data);
		read_imu_temp_batch(			&(current_struct->imu_temp_data));
		read_ir_temps_batch(			current_struct->ir_temps_data);
		read_rail_3v_batch(				&(current_struct->rail_3v_data));
		read_rail_5v_batch(				&(current_struct->rail_5v_data));
		
		// TODO: DO CHECKS FOR ERRORS (TO GENERATE ERRORS) HERE
		
		// once we've collected all the data we need to into the current struct, add the whole thing
		// if we were suspended in some period between start of this packet and here, DON'T add it
		// and go on to rewrite the current one
		if (!check_if_suspended_and_update(IDLE_DATA_TASK)) {
			// validate previous stored value in stack, getting back the next staged address we can start adding to
			current_struct = (idle_data_t*) equistack_Stage(&idle_readings_equistack);
		}
	}
	// delete this task if it ever breaks out
	vTaskDelete( NULL );
}