/*
 * Flash_Commands.h
 *
 * Created: 9/28/2017 19:30:35
 *  Author: mckenna
 */ 

#include <global.h>
#include <proc_pins.h>
#include "Direct_Pin_Commands.h"

#ifndef FLASH_COMMANDS_H_
#define FLASH_COMMANDS_H_

void set_lifepo_output_enable(bool enabled);
void flash_arm(void); // to be called at least 2ms before flashing
void flash_activate(void);
void flash_disarm(void); // to be called a minimum of 2ms after flashing

#endif /* FLASH_COMMANDS_H_ */