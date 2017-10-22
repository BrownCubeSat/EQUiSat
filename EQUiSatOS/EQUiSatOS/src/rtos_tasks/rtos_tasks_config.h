/*
 * rtos_tasks_config.h
 *
 * All constants important for the various RTOS_TASKS
 * 
 * Created: 10/2/2016 3:38:22 PM
 *  Author: mckenna
 */ 

#ifndef RTOS_TASKS_CONFIG_H
#define RTOS_TASKS_CONFIG_H

#include <global.h>
#include <assert.h>

/************************************************************************/
/* Task Properties - see below for frequencies		*/
/************************************************************************/
#define TASK_BATTERY_CHARGING_STACK_SIZE			(1024)/sizeof(portSTACK_TYPE)
#define TASK_BATTERY_CHARGING_PRIORITY				(tskIDLE_PRIORITY)

#define TASK_ANTENNA_DEPLOY_STACK_SIZE				(1024/sizeof(portSTACK_TYPE))
#define TASK_ANTENNA_DEPLOY_PRIORITY				(tskIDLE_PRIORITY)

#define TASK_WATCHDOG_STACK_SIZE					(1024/sizeof(portSTACK_TYPE))
#define TASK_WATCHDOG_STACK_PRIORITY				(tskIDLE_PRIORITY)

#define TASK_FLASH_ACTIVATE_STACK_SIZE				(1024/sizeof(portSTACK_TYPE))
#define TASK_FLASH_ACTIVATE_PRIORITY				(tskIDLE_PRIORITY)

#define TASK_TRANSMIT_STACK_SIZE					(1024/sizeof(portSTACK_TYPE))
#define TASK_TRANSMIT_PRIORITY						(tskIDLE_PRIORITY)

#define TASK_CURRENT_DATA_RD_STACK_SIZE				(1024/sizeof(portSTACK_TYPE))
#define TASK_CURRENT_DATA_RD_PRIORITY				(tskIDLE_PRIORITY)

#define TASK_FLASH_DATA_RD_STACK_SIZE				(1024/sizeof(portSTACK_TYPE))
#define TASK_FLASH_DATA_RD_PRIORITY					(tskIDLE_PRIORITY)

#define TASK_ATTITUDE_DATA_RD_STACK_SIZE			(1024/sizeof(portSTACK_TYPE))
#define TASK_ATTITUDE_DATA_DATA_RD_PRIORITY			(tskIDLE_PRIORITY)

/********************************************************************************/
/* Data reading task stack sizes - how many they can store before overwriting	*/
/********************************************************************************/
// it doesn't make sense of this to be greater than the sum of the other _MAXs
#define LAST_READING_TYPE_STACK_MAX		32
#define IDLE_STACK_MAX					2 // one stored (available for transmission), one staged (TODO: Isn't the staged one stored anyways?)
#define ATTITUDE_STACK_MAX				10
#define FLASH_STACK_MAX					10
#define FLASH_CMP_STACK_MAX				10

/************************************************************************/
/* Enum for states that represent changes in which tasks are running	*/
/************************************************************************/
typedef enum
{
	HELLO_WORLD,
	IDLE,
	LOW_POWER
} global_state_t;

/************************************************************************/
/*  Enum for all types of collected sensor readings						*/
/* (for consistency across sensor read functions)						*/
/* Based off: https://docs.google.com/a/brown.edu/spreadsheets/d/1sHQNTC5f5sg6j5DD4OKjuQykpIM3z16uetWT9YuB9PQ/edit?usp=sharing
/*	NOTE:																*/
/*	If you add/remove a type of collected data, there are several		*/
/*	things you must change:												*/
/*		- Create a batch type definition								*/
/*		- Create the required frequencies								*/
/*		- Add a new array of data to ALL of the relevant state structs  */
/*		- Add an add_*_batch_if_ready function						    */
/* NOTE: To move this somewhere, use this regex: (\w*)_DATA, --> $1		*/
/************************************************************************/
typedef enum
{
	LION_VOLTS_DATA,
	LION_CURRENT_DATA,
	LED_TEMPS_DATA,
	LIFEPO_CURRENT_DATA,
	LIFEPO_VOLTS_DATA,
	IR_DATA,
	DIODE_DATA,
	BAT_TEMP_DATA,
	IR_TEMPS_DATA,
	RADIO_TEMP_DATA,
	ACCELEROMETER_DATA,
	GYRO_DATA,
	MAGNETOMETER_DATA,
	LED_CURRENT_DATA,
	RADIO_VOLTS_DATA,
	IMU_TEMP_DATA,
	BAT_CHARGE_VOLTS_DATA,
	BAT_CHARGE_DIG_SIGS_DATA,
	DIGITAL_OUT_DATA,
	RAIL_3V_DATA,
	RAIL_5V_DATA,
	NUM_DATA_TYPES //= RAIL_5V_DATA + 1
} sensor_type_t;

/************************************************************************/
/* enum for all types of data that can be read							*/
/* (all types that will be in the 'data' section of a message packet)   */
/************************************************************************/
typedef enum
{
	IDLE_DATA,
	ATTITUDE_DATA,
	FLASH_DATA,
	FLASH_CMP
} msg_data_type_t;

/************************************************************************/
/* Enum for all tasks (for array-wise referencing for last_state, etc.) */
/************************************************************************/
typedef enum
{
	ANTENNA_DEPLOY_TASK,
	BATTERY_CHARGING_TASK,
	TRANSMIT_TASK,
	FLASH_ACTIVATE_TASK,
	CURRENT_DATA_TASK,
	ATTITUDE_DATA_TASK,
	TRANSMIT_DATA_TASK,
	FLASH_DATA_TASK,
	NUM_TASKS, //= FLASH_DATA_TASK + 1
} task_type_t;

/************************************************************************/
/* TASK EXECUTE FREQUENCIES												*/
/* These can be complex because of multi-frequency data collection		*/
/* issues... see below for details.				                        */
/************************************************************************/

/* action frequency periods in MS (some that actually have data collection are below) */
#define WATCHDOG_TASK_FREQ						1000
#define ANTENNA_DEPLOY_TASK_FREQ				1000
#define BATTERY_CHARGING_TASK_FREQ				100
#define FLASH_ACTIVATE_TASK_FREQ				60000	// 1 minute; how often to flash

#define TRANSMIT_TASK_FREQ						60000	// 1 minute; how often to transmit
#define TRANSMIT_TASK_TRANS_MONITOR_FREQ		150		// check period for transmit_task during transmission
#define TRANSMIT_TASK_CONFIRM_TIMEOUT			2000	// max "transmission time" before timing out confirmation and quit
#define TRANSMIT_TASK_MSG_REPEATS				3		// number of times to send the same transmission

/* 
 * NOTE: The idle data collection doesn't really need these constants;
 * all sensors are being read at the same frequency, but we're doing it here for consistency
 */

#define CURRENT_DATA_TASK_FREQ					1000
#define CURRENT_DATA_LOW_POWER_TASK_FREQ		10000
/* Data array lengths for current data reader task */
// for packet header
#define idle_LION_VOLTS_DATA_ARR_LEN				1
#define idle_LION_CURRENT_DATA_ARR_LEN				1
#define idle_BAT_CHARGE_VOLTS_DATA_ARR_LEN			1
#define idle_BAT_CHARGE_DIG_SIGS_DATA_ARR_LEN		1
#define idle_DIGITAL_OUT_DATA_ARR_LEN				1
// for idle data package
#define idle_BAT_TEMP_DATA_ARR_LEN					1
#define idle_RADIO_TEMP_DATA_ARR_LEN				1
#define idle_RADIO_VOLTS_DATA_ARR_LEN				1
#define idle_IMU_TEMP_DATA_ARR_LEN					1
#define idle_IR_TEMPS_DATA_ARR_LEN					1
#define idle_RAIL_3V_DATA_ARR_LEN					1
#define idle_RAIL_5V_DATA_ARR_LEN					1

/* Current data reader reads per log */
// for packet header
#define idle_LION_VOLTS_LOOPS_PER_LOG				1
#define idle_LION_CURRENT_LOOPS_PER_LOG				1
#define idle_BAT_CHARGE_VOLTS_LOOPS_PER_LOG			1
#define idle_BAT_CHARGE_DIG_SIGS_LOOPS_PER_LOG		1
#define idle_DIGITAL_OUT_LOOPS_PER_LOG				1
// for idle data package
#define idle_BAT_TEMP_LOOPS_PER_LOG					1
#define idle_RADIO_TEMP_LOOPS_PER_LOG				1
#define idle_RADIO_VOLTS_LOOPS_PER_LOG				1
#define idle_IMU_TEMP_LOOPS_PER_LOG					1
#define idle_IR_TEMPS_LOOPS_PER_LOG					1
#define idle_RAIL_3V_LOOPS_PER_LOG					1
#define idle_RAIL_5V_LOOPS_PER_LOG					1

/*
 * These may have data array lists longer than 1
 */

#define ATTITUDE_DATA_TASK_FREQ					10000
/* Data array lengths for attitude data reader task */
#define attitude_IR_DATA_ARR_LEN					1
#define attitude_DIODE_DATA_ARR_LEN					1
#define attitude_ACCELEROMETER_DATA_ARR_LEN			2
#define attitude_GYRO_DATA_ARR_LEN					1
#define attitude_MAGNETOMETER_DATA_ARR_LEN			2

/* Attitude data read task reads per log */
/* LOOPS_PER_LOG for each sensor at each state - 
	How many times the whole sensor task loop must iterate before the given sensor is logged 
	in a equistack for transmission. Note that this has an INVERSE relationship with the array length;
	more frequent sensors (with fewer loops per log) must have longer arrays.
	(see tests in rtos_tasks_config.c)
	
	NOTE: because the actual HZ frequency entered here is only computed relative 
	to the execution frequency of the reading RTOS task, it must be less than that frequency.
	*/
#define attitude_IR_LOOPS_PER_LOG					2 // = ms / [fastest log rate (ms) of any datum]
#define attitude_DIODE_LOOPS_PER_LOG				2 // = ms / [fastest log rate (ms) of any datum]
#define attitude_ACCELEROMETER_LOOPS_PER_LOG		1 // = ms / [fastest log rate (ms) of any datum]
#define attitude_GYRO_LOOPS_PER_LOG					2 // = ms / [fastest log rate (ms) of any datum]
#define attitude_MAGNETOMETER_LOOPS_PER_LOG			1 // = ms / [fastest log rate (ms) of any datum]


#define FLASH_DATA_TASK_FREQ					10
/* Data array lengths for flash BURST segment of data reader task */
#define flash_LED_TEMPS_DATA_ARR_LEN			10
#define flash_LIFEPO_CURRENT_DATA_ARR_LEN		10
#define flash_LIFEPO_VOLTS_DATA_ARR_LEN			10
#define flash_LED_CURRENT_DATA_ARR_LEN			10

/* Flash BURST segment of data reader task reads per log */
/* ibid */
#define flash_LED_TEMPS_LOOPS_PER_LOG			1 // = ms / [fastest log rate (ms) of any datum]
#define flash_LIFEPO_CURRENT_LOOPS_PER_LOG		1 // = ms / [fastest log rate (ms) of any datum]
#define flash_LIFEPO_VOLTS_LOOPS_PER_LOG		1 // = ms / [fastest log rate (ms) of any datum]
#define flash_LED_CURRENT_LOOPS_PER_LOG			1 // = ms / [fastest log rate (ms) of any datum]

/* Data array lengths for flash COMPARISON segment of data reader task */
#define flash_LED_TEMPS_AVG_DATA_ARR_LEN		1
#define flash_LIFEPO_CURRENT_AVG_DATA_ARR_LEN	1
#define flash_LIFEPO_VOLTS_AVG_DATA_ARR_LEN		1
#define flash_LED_CURRENT_AVG_DATA_ARR_LEN		1

/* Flash COMPARISON segment of data reader task reads per log */
/* ibid */
#define flash_LED_TEMPS_AVG_LOOPS_PER_LOG		1 // defined to be 1
#define flash_LIFEPO_CURRENT_AVG_LOOPS_PER_LOG	1
#define flash_LIFEPO_VOLTS_AVG_LOOPS_PER_LOG	1
#define flash_LED_CURRENT_AVG_LOOPS_PER_LOG		1 

/*
 * A function to make sure that the constants defined here are internally consistent.
 */
void assert_rtos_constants(void);

#endif