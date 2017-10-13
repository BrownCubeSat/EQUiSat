/*
 * Sensor_Structs.h
 *
 * Created: 9/27/2016 9:42:32 PM
 *  Author: rj16
 */ 

#ifndef SENSOR_STRUCTS_H
#define SENSOR_STRUCTS_H

#include <global.h>

typedef uint16_t lion_volts_batch			[2];
typedef uint16_t lion_current_batch			[2];
typedef uint16_t led_temps_batch			[4];
typedef uint16_t lifepo_current_batch		[4];
typedef uint16_t lifepo_volts_batch			[4];
typedef uint16_t bat_temp_batch				[4];
typedef uint16_t ir_temps_batch				[6];
typedef uint16_t radio_volts_batch			[2];
typedef uint16_t bat_charge_volts_batch		[14];
typedef uint16_t bat_charge_dig_sigs_batch;	// single value
typedef uint16_t digital_out_batch;			// single value
typedef uint16_t ir_batch					[12];
typedef uint8_t diode_batch					[6];
typedef uint16_t led_current_batch			[4];
typedef uint16_t magnetometer_batch			[3];
typedef struct imu_batch
{
	// one for each axis, for each sensor
	uint16_t accelerometer[3];
	uint16_t gyro[3];
	uint16_t magnetometer[3];
} imu_batch;
typedef uint8_t radio_temp_batch;			// single value

#endif
